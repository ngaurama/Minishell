/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 19:44:08 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	read_heredoc(const char *delimiter, int out_fd, t_shell *shell,
			int expand)
{
	char	*line;
	int		fd;

	g_signal_num = 0;
	rl_event_hook = rl_hook;
	signal(SIGINT, handle_signal_heredoc);
	while (1)
	{
		line = readline("> ");
		if (stop_heredoc(line, delimiter))
			break ;
		if (g_signal_num == SIGINT)
		{
			free(line);
			close(out_fd);
			fd = 2;
			while (++fd < 1024)
				close(fd);
			free_shell(shell);
			exit(130);
		}
		write_heredoc_line(out_fd, line, shell, expand);
		free(line);
	}
	rl_event_hook = NULL;
}

int	handle_heredoc(const char *delimiter, t_shell *shell, int expand)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	int 	fd; 

	status = 0;
	//signal(SIGINT, SIG_IGN);
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
	{
		close(pipefd[0]);
		read_heredoc(delimiter, pipefd[1], shell, expand);
		close(pipefd[1]);
		fd = 2;
		while (++fd < 1024)
			close(fd);
		free_shell(shell);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipefd[0]);
			g_signal_num = SIGINT;
			shell->exit_status = 130;
			return (-1);
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			close(pipefd[0]);
			g_signal_num = SIGINT;
			shell->exit_status = 130;
			return (-1);
		}
		return (pipefd[0]);
	}
}

int	manage_heredocs(t_redir *redir, t_shell *shell)
{
	int	fd;

	fd = -1;
	while (redir)
	{
		if (redir->type == T_HEREDOC)
			fd = handle_heredoc_input(redir, shell);
		else
			return (1);
		if (fd == -1)
		{
			//perror(redir->filename);
			shell->redir_err = 130;
			return (1);
		}
		redir = redir->next;
	}
	if (!shell->redir_err && fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	handle_input_redirection(t_redir *redir, t_shell *shell)
{
	int	fd;

	fd = -1;
	while (redir)
	{
		if (redir->filename)
			fd = handle_redirect_in_file(redir->filename);
		else
			fd = redir->fd;
		if (fd == -1)
			return (handle_inredir_error(redir, shell));
		redir = redir->next;
	}
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	return (0);
}

int	handle_output_redirection(t_redir *redir)
{
	int	fd;
	int	flags;

	while (redir)
	{
		if (redir->type == T_REDIRECT_OUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redir->type == T_APPEND)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
		{
			ft_putstr_fd("Invalid output redirection type\n", STDERR_FILENO);
			return (1);
		}
		fd = open(redir->filename, flags, 0644);
		if (fd == -1)
			return (perror(redir->filename), 1);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	redirection(t_command *cmd, t_shell *shell)
{
	manage_heredocs(cmd->heredocs, shell);
	if (handle_input_redirection(cmd->infiles, shell) != 0)
		return (1);
	if (handle_output_redirection(cmd->outfiles) != 0)
		return (1);
	return (shell->redir_err);
}
