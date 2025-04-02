/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:53:58 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/02 12:17:48 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit(t_shell *shell, int exit_code)
{
	if (shell)
		free_shell(shell);
	exit(exit_code);
}

void 	setup_child_pipes(int prev_pipe_read, int pipefd[2], t_command *cmd)
{
	if (prev_pipe_read != -1 && !cmd->infiles && !cmd->heredocs)
	{
		dup2(prev_pipe_read, STDIN_FILENO);
		close(prev_pipe_read);
	}
	if (cmd->pipe && !cmd->outfiles)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void 	execute_child_pipes(t_shell *shell, t_command *cmd)
{
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		free_and_exit(shell, 127);
	if (redirection(cmd, shell) != 0)
		free_and_exit(shell, 1);
	if (check_built_in(cmd))
	{
		execute_built_in(shell, cmd);
		free_and_exit(shell, 0);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (find_full_path(shell, cmd->args[0]) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			free_and_exit(shell, 127);
		}
		execve(shell->full_path, cmd->args, shell->env);
		perror("execve failed");
		free_and_exit(shell, 1);
	}
}
