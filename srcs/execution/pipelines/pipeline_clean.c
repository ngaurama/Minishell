/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_clean.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 14:56:13 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 17:40:08 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	read_and_write_stderr(int fd)
{
	char	buf[1024];
	ssize_t	n;
	int		saved_stderr;

	saved_stderr = dup(STDERR_FILENO);
	if (saved_stderr == -1)
	{
		perror("dup stderr");
		return ;
	}
	n = read(fd, buf, sizeof(buf) - 1);
	while (n > 0)
	{
		write(saved_stderr, buf, n);
		n = read(fd, buf, sizeof(buf) - 1);
	}
	close(fd);
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stderr);
}

int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	status;
	int	last_status;
	int	i;

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
	return (last_status);
}

int	pipeline_cleanup(int buffer[2], int prev_pipe_read, pid_t *pids,
		int cmd_count)
{
	int	status;

	status = wait_for_children(pids, cmd_count);
	close(buffer[1]);
	read_and_write_stderr(buffer[0]);
	if (prev_pipe_read != -1)
		close(prev_pipe_read);
	return (status);
}
