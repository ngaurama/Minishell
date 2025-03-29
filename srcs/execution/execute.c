/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/30 00:02:15 by ngaurama         ###   ########.fr       */
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
	if (execve(shell->full_path, shell->cmds->args, shell->env) == -1)
	{
		perror("execve failed");
		exit(1);
	}
}

int execute_command(t_shell *shell)
{
	int	path_result;
	
	if (!shell || !shell->cmds || !shell->cmds->args || !shell->cmds->args[0])
		return (1);
	if (shell->cmds->args[0][0] == '\0') 
	{
        ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
        shell->exit_status = 127;
        return (1);
    }
	int saved_stdin, saved_stdout;
	if (!save_fds(&saved_stdin, &saved_stdout))
		return (1);
	if (redirection(shell->cmds, shell) != 0)
		return (restore_fds(saved_stdin, saved_stdout), 1);
	path_result = find_full_path(shell, shell->cmds->args[0]);
	if (path_result == 2)
		return (restore_fds(saved_stdin, saved_stdout), 1);
	if (path_result != 0)
		return (handle_command_not_found(shell), restore_fds(saved_stdin, saved_stdout), 1);
	pid_t pid = fork();
	if (pid == 0)
		execute_child_process(shell);
	else if (pid > 0)
		shell->pid = pid;
	else
		return (perror("fork failed"), restore_fds(saved_stdin, saved_stdout), 1);
	restore_fds(saved_stdin, saved_stdout);
	return (0);
}
