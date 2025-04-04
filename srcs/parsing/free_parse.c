/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:23:24 by npbk              #+#    #+#             */
/*   Updated: 2025/04/04 22:24:54 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_redirections(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs;
		redirs = redirs->next;
		if (tmp && tmp->filename)
			free(tmp->filename);
		if (tmp->fd != -1)
			close (tmp->fd);
		tmp->filename = NULL;
		free(tmp);
	}
}

void	free_commands(t_command *cmds)
{
	t_command	*tmp;
	t_command	*next;
	int			i;

	if (!cmds)
		return ;
	while (cmds)
	{
		tmp = cmds;
		next = cmds->next;
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i])
				free(tmp->args[i++]);
			free(tmp->args);
		}
		free_redirections(tmp->infiles);
		free_redirections(tmp->outfiles);
		if (tmp->heredocs)
			free_redirections(tmp->heredocs);
		free(tmp);
		cmds = next;
	}
}

void	free_arguments(t_arg *args)
{
	t_arg	*tmp;

	while (args)
	{
		tmp = args;
		args = args->next;
		free(tmp->value);
		free(tmp);
	}
}

void	free_tokenizer(t_tokenizer *tok)
{
	if (tok->token)
		free(tok->token);
	tok->token = NULL;
}
