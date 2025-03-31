/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 16:52:48 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	else
		shell->exit_status = 1;
}

int	save_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup failed");
		return (0);
	}
	return (1);
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	handle_command_not_found(t_shell *shell)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(shell->cmds->args[0], STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	shell->exit_status = 127;
}

void	execute_child_process(t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execve(shell->full_path, shell->cmds->args, shell->env) == -1)
	{
		perror("execve failed");
		exit(1);
	}
}

static int setup_execution(t_shell *shell, int *saved_stdin, int *saved_stdout)
{
    if (!shell || !shell->cmds || !shell->cmds->args || !shell->cmds->args[0])
        return (shell->exit_status = 1, 1);
    if (shell->cmds->args[0][0] == '\0')
    {
        ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
        return (shell->exit_status = 127, 1);
    }
    if (!save_fds(saved_stdin, saved_stdout) || redirection(shell->cmds, shell) != 0)
        return (shell->exit_status = 1, 1);
    return (0);
}

static int handle_path_failure(t_shell *shell, int path_result)
{
    if (path_result == 2)
        return (shell->exit_status = 126, 1);
    if (path_result == 1)
    {
        if (access(shell->cmds->args[0], F_OK) == 0)
            shell->exit_status = 126;
        else
            handle_command_not_found(shell);
    }
    else
        handle_command_not_found(shell);
    return (1);
}

static int fork_and_execute(t_shell *shell)
{
    pid_t pid = fork();
    if (pid == 0)
        execute_child_process(shell);
    if (pid > 0)
    {
        int status;
        shell->pid = pid;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status))
        {
            int sig = WTERMSIG(status);
            if (sig == SIGQUIT)
                ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
            else if (sig == SIGINT)
                ft_putstr_fd("\n", STDERR_FILENO);
        }
        update_exit_status(shell, status);
        return (0);
    }
    perror("fork failed");
    return (shell->exit_status = 1, 1);
}

int execute_command(t_shell *shell)
{
    int saved_stdin, saved_stdout;
    int path_result;

    if (setup_execution(shell, &saved_stdin, &saved_stdout))
        return (1);
    path_result = find_full_path(shell, shell->cmds->args[0]);
    if (path_result != 0)
    {
        handle_path_failure(shell, path_result);
        return (restore_fds(saved_stdin, saved_stdout), 1);
    }
    if (fork_and_execute(shell))
        return (restore_fds(saved_stdin, saved_stdout), 1);
    restore_fds(saved_stdin, saved_stdout);
    return (0);
}
