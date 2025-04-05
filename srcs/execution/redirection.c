/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/05 11:02:41 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	manage_heredocs(t_redir *redir, t_shell *shell)
{
	int	fd;

	fd = -1;
	while (redir)
	{
		if (redir->type == T_HEREDOC)
			fd = handle_heredoc(redir, shell);
		else
			return (1);
		if (fd == -1)
		{
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
