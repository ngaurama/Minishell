/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 16:42:21 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	setup_execution(t_shell *shell, int *saved_stdin, int *saved_stdout)
{
	if (!shell || !shell->cmds || !shell->cmds->args || !shell->cmds->args[0])
		return (shell->exit_status = 1, 1);
	if (shell->cmds->args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		return (shell->exit_status = 127, 1);
	}
	if (!save_fds(saved_stdin, saved_stdout))
		return (shell->exit_status = 1, 1);
	if (redirection(shell->cmds, shell) != 0 || shell->redir_err)
	{
		shell->exit_status = 1;
		restore_fds(*saved_stdin, *saved_stdout);
		shell->redir_err = 0;
		return (1);
	}
	return (0);
}

static int	handle_path_failure(t_shell *shell, int path_result)
{
	if (path_result == 2)
		return (shell->exit_status = 126, 1);
	if (path_result == 1)
	{
		if (!shell->err_printed && \
			access(shell->cmds->args[0], F_OK) == 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(shell->cmds->args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			shell->err_printed = 1;
			shell->exit_status = 127;
		}
		else if (!shell->err_printed)
			handle_command_not_found(shell);
	}
	else if (!shell->err_printed)
		handle_command_not_found(shell);
	return (1);
}

static void	handle_parent_process(pid_t pid, t_shell *shell)
{
	int	status;
	int	sig;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals(shell);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		else if (sig == SIGINT)
			ft_putstr_fd("\n", STDERR_FILENO);
	}
	update_exit_status(shell, status);
}

static int	fork_and_execute(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_child_process(shell);
	if (pid > 0)
	{
		shell->pid = pid;
		handle_parent_process(pid, shell);
		return (0);
	}
	perror("fork failed");
	return (shell->exit_status = 1, 1);
}

int	execute_command(t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	int	path_result;

	shell->err_printed = 0;
	if (setup_execution(shell, &saved_stdin, &saved_stdout))
		return (restore_fds(saved_stdin, saved_stdout), 1);
	path_result = find_full_path(shell, shell->cmds->args[0]);
	if (path_result != 0)
	{
		handle_path_failure(shell, path_result);
		shell->redir_err = 0;
		return (restore_fds(saved_stdin, saved_stdout), 1);
	}
	if (fork_and_execute(shell))
	{
		shell->redir_err = 0;
		restore_fds(saved_stdin, saved_stdout);
		return (1);
	}
	restore_fds(saved_stdin, saved_stdout);
	shell->redir_err = 0;
	return (0);
}
