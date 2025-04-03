/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 18:32:32 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	pipeline_handle_special(t_shell *shell, t_command **cmd, int pipefd[2])
{
	pid_t	writer_pid;
	pid_t	reader_pid;
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	writer_pid = fork();
	if (writer_pid == 0)
	{
		close(pipefd[0]);
		setup_child_pipes(-1, pipefd, *cmd);
		execute_child_pipes(shell, *cmd);
		exit(0);
	}
	else if (writer_pid > 0)
	{
		close(pipefd[1]);
		waitpid(writer_pid, &status, 0);
		*cmd = (*cmd)->next;
		if (!*cmd)
			return ;
		reader_pid = fork();
		if (reader_pid == 0)
		{
			setup_child_pipes(pipefd[0], pipefd, *cmd);
			close(pipefd[1]);
			execute_child_pipes(shell, *cmd);
			exit(0);
		}
		else if (reader_pid > 0)
		{
			close(pipefd[0]);
			*cmd = (*cmd)->next;
		}
		else
		{
			perror("fork failed");
			close(pipefd[0]);
		}
	}
	else
	{
		perror("fork failed");
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	pipeline_handle_regular(t_shell *shell, t_command *cmd, int buffer[2],
		int *prev_pipe_read)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(buffer[0]);
		dup2(buffer[1], STDERR_FILENO);
		close(buffer[1]);
		setup_child_pipes(*prev_pipe_read, cmd->pipefd, cmd);
		execute_child_pipes(shell, cmd);
	}
	else if (pid > 0)
	{
		if (*prev_pipe_read != -1)
			close(*prev_pipe_read);
		if (cmd->pipe)
		{
			close(cmd->pipefd[1]);
			*prev_pipe_read = cmd->pipefd[0];
		}
	}
	else
		handle_fork_error(buffer);
}

void	pipeline(t_shell *shell)
{
	int			buffer[2] = {-1, -1};
	int			prev_pipe_read;
	pid_t		*pids;
	int			cmd_count;
	t_command	*cmd;

	prev_pipe_read = -1;
	pids = NULL;
	cmd_count = 0;
	cmd = shell->cmds;
	while (cmd && ++cmd_count)
		cmd = cmd->next;
	if (cmd_count == 0)
		return ;
	pids = ft_calloc(cmd_count, sizeof(pid_t));
	if (!pids)
	{
		perror("malloc failed");
		shell->exit_status = 1;
		return ;
	}
	if (pipe(buffer) == -1)
	{
		perror("pipe failed");
		free(pids);
		shell->exit_status = 1;
		return ;
	}
	pipeline_process(shell, buffer, &prev_pipe_read, pids);
	shell->exit_status = pipeline_cleanup(buffer, prev_pipe_read, pids,
			cmd_count);
	free(pids);
}
