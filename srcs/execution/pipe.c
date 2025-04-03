/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 14:03:32 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pipeline_handle_special(t_shell *shell, t_command **cmd, int pipefd[2])
{
	pid_t	writer_pid;
	pid_t	reader_pid;

	writer_pid = fork();
	if (writer_pid == 0)
	{
		setup_child_pipes(-1, pipefd, *cmd);
		execute_child_pipes(shell, *cmd);
	}
	else if (writer_pid > 0)
	{
		*cmd = (*cmd)->next;
		reader_pid = fork();
		if (reader_pid == 0)
		{
			setup_child_pipes(-1, pipefd, *cmd);
			execute_child_pipes(shell, *cmd);
		}
		else if (reader_pid > 0)
			*cmd = (*cmd)->next;
		else
			perror("fork failed");
	}
	else
		perror("fork failed");
}

void	pipeline_handle_regular(t_shell *shell, t_command *cmd,
			int buffer[2], int *prev_pipe_read)
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

//Pipeline process start
void	pipeline_process(t_shell *shell, int buffer[2], int *prev_pipe_read,
	pid_t *pids)
{
	t_command	*cmd;
	int			pipefd[2];
	int			i;

	cmd = shell->cmds;
	i = 0;
	while (cmd) 
	{
		if (cmd->outfiles && cmd->pipe && cmd->next) 
		{
			pipeline_handle_special(shell, &cmd, pipefd);
			continue ;
		}
		if (cmd->pipe && pipe(pipefd) == -1) 
		{
			perror("pipe");
			close(buffer[0]);
			close(buffer[1]);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == 0) 
		{
			close(buffer[0]);
			dup2(buffer[1], STDERR_FILENO);
			close(buffer[1]);
			setup_child_pipes(*prev_pipe_read, pipefd, cmd);
			execute_child_pipes(shell, cmd);
		} 
		else if (pids[i] > 0) 
		{
			if (*prev_pipe_read != -1)
				close(*prev_pipe_read);
			if (cmd->pipe) 
			{
				close(pipefd[1]);
				*prev_pipe_read = pipefd[0];
			}
			i++;
		} 
		else 
		{
			perror("fork failed");
			close(buffer[0]);
			close(buffer[1]);
		}
		cmd = cmd->next;
	}
}
//Pipeline process end


//Pipecleanup start
void	read_and_write_stderr(int fd)
{
	char buf[1024];
	ssize_t n;

	n = read(fd, buf, sizeof(buf) - 1);
	while (n > 0)
	{
		write(STDERR_FILENO, buf, n);
		n = read(fd, buf, sizeof(buf) - 1);
	}
	close(fd);
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int status;
	int last_status;
	int i;

	i = -1;
	last_status = 0;
	while (++i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
	}
	return last_status;
}

int	pipeline_cleanup(int buffer[2], int prev_pipe_read, pid_t *pids, int cmd_count)
{
	close(buffer[1]);
	read_and_write_stderr(buffer[0]);
	if (prev_pipe_read != -1)
		close(prev_pipe_read);
	return wait_for_children(pids, cmd_count);
}
//Pipecleanup end

void	pipeline(t_shell *shell)
{
	int		buffer[2];
	int		prev_pipe_read;
	pid_t	*pids;
	int		cmd_count;
	t_command	*cmd;

	cmd_count = 0;
	cmd = shell->cmds;
	while (cmd) 
	{
		cmd_count++;
		cmd = cmd->next;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids) 
	{
		perror("malloc failed");
		shell->exit_status = 1;
		return;
	}
	pipeline_init(shell, buffer, &prev_pipe_read);
	pipeline_process(shell, buffer, &prev_pipe_read, pids);
	shell->exit_status = pipeline_cleanup(buffer, prev_pipe_read, pids, cmd_count);
	free(pids);
}
