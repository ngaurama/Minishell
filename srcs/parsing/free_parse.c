/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:23:24 by npbk              #+#    #+#             */
/*   Updated: 2025/03/19 19:26:03 by npbk             ###   ########.fr       */
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
        free(tmp->filename);
        free(tmp);
    }
}

void free_commands(t_command *cmds)
{
    t_command *tmp;
    int i;

    while (cmds)
    {
        tmp = cmds;
        for (i = 0; cmds->args[i]; i++)
            free(cmds->args[i]);
        free_redirections(cmds->infiles);
        free_redirections(cmds->outfiles);
        cmds = cmds->next;
        free(tmp);
    }
}

void free_tokens(t_arg *tokens)
{
    t_arg *tmp;

    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        free(tmp->value);
        free(tmp);
    }
}
