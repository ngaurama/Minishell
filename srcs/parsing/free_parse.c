/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:23:24 by npbk              #+#    #+#             */
/*   Updated: 2025/03/22 04:54:40 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_expanded_tokens(char *expanded_tilde, char *expanded_var,
	 t_tokenizer *tok)
{
	if (expanded_var != expanded_tilde)
		free(expanded_var);
	if (expanded_tilde != tok->token)
		free(expanded_tilde);
}

static void free_redirections(t_redir *redirs)
{
    t_redir *tmp;

    while (redirs)
    {
        tmp = redirs;
        redirs = redirs->next;
        if (tmp->filename)
            free(tmp->filename);
        free(tmp);
    }
}

void free_commands(t_command *cmds)
{
	t_command *tmp;
	t_command *next;
	int i;

	while (cmds)
	{
		tmp = cmds;
		next = cmds->next;
		i = 0;
		if (tmp->args)
		{
			while (tmp->args[i])
				free(tmp->args[i++]);
		}
		free_redirections(tmp->infiles);
		free_redirections(tmp->outfiles);
		free(tmp);
		cmds = next;
	}
}

void free_arguments(t_arg *args)
{
    t_arg *tmp;

    while (args)
    {
        tmp = args;
        args = args->next;
        free(tmp->value);
        free(tmp);
    }
}
