/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 22:10:58 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static pid_t	pipeline_handle_special(t_shell *shell, t_command **cmd,
		int pipefd[2])
{
	pid_t	writer_pid;
	pid_t	reader_pid;

	reader_pid = -1;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
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
			setup_child_pipes(pipefd[0], pipefd, *cmd);
			close(pipefd[1]);
			execute_child_pipes(shell, *cmd);
		}
		else if (reader_pid > 0)
		{
			*cmd = (*cmd)->next;
			close(pipefd[0]);
			close(pipefd[1]);
			waitpid(writer_pid, NULL, 0);
		}
	}
	else
		perror("fork failed");
	return (reader_pid);
}

static pid_t	pipeline_handle_regular(t_shell *shell, t_command *cmd,
		int buffer[2], int *prev_pipe_read)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		close(buffer[0]);
		if (dup2(buffer[1], STDERR_FILENO) == -1)
		{
			perror("dup2 stderr");
			exit(1);
		}
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
	return (pid);
}

static pid_t	pipeline_process(t_shell *shell, int buffer[2],
		int *prev_pipe_read)
{
	t_command	*cmd;
	pid_t		last_pid;

	cmd = shell->cmds;
	last_pid = -1;
	while (cmd)
	{
		if (cmd->outfiles && cmd->pipe && cmd->next)
		{
			last_pid = pipeline_handle_special(shell, &cmd, cmd->pipefd);
			continue ;
		}
		if (cmd->pipe && pipe(cmd->pipefd) == -1)
		{
			perror("pipe");
			close(buffer[0]);
			close(buffer[1]);
			return (-1);
		}
		last_pid = pipeline_handle_regular(shell, cmd, buffer, prev_pipe_read);
		cmd = cmd->next;
	}
	return (last_pid);
}

static int	pipeline_cleanup(int buffer[2], int prev_pipe_read, pid_t last_pid)
{
	int	status;
	int	sig;

	status = 0;
	close(buffer[1]);
	read_and_write_stderr(buffer[0]);
	if (prev_pipe_read != -1)
		close(prev_pipe_read);
	if (last_pid > 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			status = 128 + sig;
			if (sig == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			else if (sig == SIGINT)
				ft_putstr_fd("\n", STDERR_FILENO);
		}
	}
	return (status);
}

void	pipeline(t_shell *shell)
{
	int		buffer[2];
	int		prev_pipe_read;
	pid_t	last_pid;

	pipeline_init(shell, buffer, &prev_pipe_read);
	last_pid = pipeline_process(shell, buffer, &prev_pipe_read);
	shell->exit_status = pipeline_cleanup(buffer, prev_pipe_read, last_pid);
}
