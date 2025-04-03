/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pipes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:55:34 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 18:32:53 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	init_pipeline(t_pipeline *pl, t_shell *shell, int buffer[2],
		int *prev_pipe_read)
{
	pl->shell = shell;
	pl->buffer[0] = buffer[0];
	pl->buffer[1] = buffer[1];
	pl->prev_pipe_read = prev_pipe_read;
	pl->cmd = shell->cmds;
	pl->i = 0;
}

static void	handle_child_process(t_pipeline *pl)
{
	close(pl->buffer[0]);
	dup2(pl->buffer[1], STDERR_FILENO);
	close(pl->buffer[1]);
	setup_child_pipes(*pl->prev_pipe_read, pl->pipefd, pl->cmd);
	execute_child_pipes(pl->shell, pl->cmd);
}

static void	handle_parent_process(t_pipeline *pl)
{
	if (*pl->prev_pipe_read != -1)
		close(*pl->prev_pipe_read);
	if (pl->cmd->pipe)
	{
		close(pl->pipefd[1]);
		*pl->prev_pipe_read = pl->pipefd[0];
	}
	pl->i++;
}

static void	process_command(t_pipeline *pl)
{
	if (pl->cmd == NULL)
		return ;
	if (pl->cmd->outfiles && pl->cmd->pipe && pl->cmd->next)
	{
		pipeline_handle_special(pl->shell, &pl->cmd, pl->pipefd);
		return ;
	}
	if (pl->cmd->pipe && pipe(pl->pipefd) == -1)
	{
		perror("pipe");
		close(pl->buffer[0]);
		close(pl->buffer[1]);
		return ;
	}
	pl->pids[pl->i] = fork();
	if (pl->pids[pl->i] == 0)
		handle_child_process(pl);
	else if (pl->pids[pl->i] > 0)
		handle_parent_process(pl);
	else
	{
		perror("fork failed");
		close(pl->buffer[0]);
		close(pl->buffer[1]);
	}
}

void	pipeline_process(t_shell *shell, int buffer[2], int *prev_pipe_read,
		pid_t *pids)
{
	t_pipeline	pl;

	pl.pids = pids;
	init_pipeline(&pl, shell, buffer, prev_pipe_read);
	while (pl.cmd != NULL)
	{
		process_command(&pl);
		if (pl.cmd == NULL)
			break ;
		pl.cmd = pl.cmd->next;
	}
}
