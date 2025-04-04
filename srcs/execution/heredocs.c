/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 12:16:24 by npbk              #+#    #+#             */
/*   Updated: 2025/04/04 20:59:40 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_redir	*create_heredoc_redir(int fd)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		perror("malloc");
		return (NULL);
	}
	redir->type = T_REDIRECT_IN;
	redir->fd = fd;
	redir->filename = NULL;
	redir->next = NULL;
	redir->src_token = NULL;
	return (redir);
}

static int	get_heredoc_fd(t_redir *heredocs, t_shell *shell)
{
	int	fd;
	int	expand;

	fd = -1;
	while (heredocs)
	{
		expand = (heredocs->src_token && heredocs->src_token->quoted == 0);
		fd = handle_heredoc(heredocs->filename, shell, expand);
		if (fd == -1)
			return (-1);
		heredocs = heredocs->next;
	}
	return (fd);
}

static t_redir	*create_heredoc_input_fd(t_redir *heredocs, t_shell *shell)
{
	int	fd;

	fd = get_heredoc_fd(heredocs, shell);
	if (fd == -1)
	{
		free_redirections(heredocs);
		return (NULL);
	}
	return (create_heredoc_redir(fd));
}

void	preprocess_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir	*new_in;

	while (cmd)
	{
		if (cmd->heredocs)
		{
			new_in = create_heredoc_input_fd(cmd->heredocs, shell);
			if (new_in && cmd->heredocs)
				free_redirections(cmd->heredocs);
			cmd->heredocs = NULL;
			if (cmd->infiles)
			{
				free_redirections(cmd->infiles);
				cmd->infiles = NULL;
			}
			if (new_in)
				cmd->infiles = new_in;
		}
		cmd = cmd->next;
	}
}
