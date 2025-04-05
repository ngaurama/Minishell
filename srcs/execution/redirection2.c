/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 11:01:51 by npagnon           #+#    #+#             */
/*   Updated: 2025/04/05 12:16:44 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	read_heredoc(t_redir *redir, int out_fd, t_shell *shell)
{
	char	*line;

	g_signal_num = 0;
	rl_event_hook = rl_hook;
	signal(SIGINT, handle_signal_heredoc);
	while (1)
	{
		line = readline("> ");
		if (g_signal_num == SIGINT)
		{
			free(line);
			close(out_fd);
			close_extra_fds(2);
			free_shell(shell);
			exit(130);
		}
		if (stop_heredoc(line, redir->filename, redir->src_token, shell))
			break ;
		write_heredoc_line(out_fd, line, shell, redir->src_token);
		free(line);
	}
	rl_event_hook = NULL;
	close(out_fd);
}

static void	heredoc_child_process(t_redir *redir, int *pipefd, t_shell *shell)
{
	close(pipefd[0]);
	read_heredoc(redir, pipefd[1], shell);
	close(pipefd[1]);
	close_extra_fds(2);
	free_shell(shell);
	exit(0);
}

static int	handle_heredoc_parent(int read_fd, int status, t_shell *shell)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		g_signal_num = SIGINT;
		shell->exit_status = 130;
		return (1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(read_fd);
		g_signal_num = SIGINT;
		shell->exit_status = 130;
		return (1);
	}
	return (0);
}

int	handle_heredoc(t_redir *redir, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	status = 0;
	signal(SIGINT, SIG_IGN);
	if (pipe(pipefd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
		heredoc_child_process(redir, pipefd, shell);
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if (handle_heredoc_parent(pipefd[0], status, shell))
			return (-1);
	}
	return (pipefd[0]);
}
