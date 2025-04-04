/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 20:32:25 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	save_and_redirect_fds(int *saved_stdin, int *saved_stdout, t_command *cmd,
		t_shell *shell)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup failed");
		return (1);
	}
	if (redirection(cmd, shell) != 0)
	{
		close(*saved_stdin);
		close(*saved_stdout);
		return (1);
	}
	return (0);
}

int	setup_builtin_redirections(t_shell *shell, t_command *cmd, int *saved_stdin,
		int *saved_stdout)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	if (!cmd->pipe)
	{
		if (save_and_redirect_fds(saved_stdin, saved_stdout, cmd, shell) != 0)
		{
			shell->exit_status = 1;
			shell->redir_err = 0;
			return (1);
		}
	}
	if (shell->redir_err)
	{
		shell->exit_status = 1;
		if (*saved_stdin != -1 && *saved_stdout != -1)
			restore_fds(*saved_stdin, *saved_stdout);
		shell->redir_err = 0;
		return (1);
	}
	return (0);
}

// execute builtins

static int	handle_redirection_built_in(t_shell *shell, t_command *cmd,
		int *saved_stdin, int *saved_stdout)
{
	if (!cmd->pipe)
	{
		if (save_and_redirect_fds(saved_stdin, saved_stdout, cmd, shell) != 0)
		{
			shell->exit_status = 1;
			shell->redir_err = 0;
			return (1);
		}
	}
	return (0);
}

static int	handle_redir_error(t_shell *shell, int saved_stdin,
		int saved_stdout)
{
	if (shell->redir_err)
	{
		shell->exit_status = 1;
		if (saved_stdin != -1 && saved_stdout != -1)
			restore_fds(saved_stdin, saved_stdout);
		shell->redir_err = 0;
		return (1);
	}
	return (0);
}

int	execute_built_in(t_shell *shell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (handle_redirection_built_in(shell, cmd, &saved_stdin, &saved_stdout))
		return (1);
	if (handle_redir_error(shell, saved_stdin, saved_stdout))
		return (1);
	if (!cmd->args[0])
	{
		shell->exit_status = 1;
		return (1);
	}
	choose_builtin(shell, cmd);
	if (saved_stdin != -1 && saved_stdout != -1)
		restore_fds(saved_stdin, saved_stdout);
	return (shell->exit_status);
}
