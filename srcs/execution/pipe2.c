/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:33:56 by npagnon           #+#    #+#             */
/*   Updated: 2025/04/05 13:29:05 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	setup_child_pipes(int prev_pipe_read, int pipefd[2], t_command *cmd)
{
	if (prev_pipe_read != -1 && !cmd->infiles && !cmd->heredocs)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
		close(prev_pipe_read);
	}
	if (cmd->pipe && !cmd->outfiles)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(1);
		}
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	execute_child_pipes(t_shell *shell, t_command *cmd)
{
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		free_and_exit(shell, 127);
	if (redirection(cmd, shell) != 0)
		free_and_exit(shell, 1);
	if (check_built_in(cmd))
		free_and_exit(shell, execute_built_in(shell, cmd));
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (find_full_path(shell, cmd->args[0]) != 0)
			child_error_and_exit(shell, cmd->args[0]);
		execve(shell->full_path, cmd->args, shell->env);
		perror("execve failed");
		free_and_exit(shell, 1);
	}
}

void	launch_writer_child(t_shell *shell, t_command *cmd, int pipefd[2])
{
	setup_child_pipes(-1, pipefd, cmd);
	execute_child_pipes(shell, cmd);
}

void	launch_reader_child(t_shell *shell, t_command *cmd, int pipefd[2])
{
	setup_child_pipes(pipefd[0], pipefd, cmd);
	close(pipefd[1]);
	execute_child_pipes(shell, cmd);
}

void	handle_parent_cleanup(t_command **cmd, int pipefd[2], pid_t writer_pid)
{
	*cmd = (*cmd)->next;
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(writer_pid, NULL, 0);
}
