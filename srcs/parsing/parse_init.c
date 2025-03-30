/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:18:28 by npbk              #+#    #+#             */
/*   Updated: 2025/03/30 21:46:00 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_arg	*add_token(t_arg *head, char *token, int type, int quoted)
{
	t_arg	*new;
	t_arg	*tmp;

	if (!token || (*token == '\0' && !quoted))
		return (head);
	//printf("Token created: [%s] (type: %d)\n", token, type); // DEBUG
	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->value = ft_strdup(token);
	new->type = type;
	new->quoted = quoted;
	new->next = NULL;
	if (!head)
		return (new);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (head);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->heredocs = NULL;
	cmd->append = 0;
	cmd->pipe = 0;
	cmd->next = NULL;
	return (cmd);
}

int	ensure_token_capacity(t_tokenizer *tok, int extra)
{
	char	*new_token;
	int		new_capacity;

	if (tok->j + extra < tok->token_capacity)
		return (1);
	new_capacity = tok->token_capacity * 2 + extra;
	new_token = malloc(new_capacity);
	if (!new_token)
		return (0);
	ft_strncpy(new_token, tok->token, tok->j);
	free(tok->token);
	tok->token = new_token;
	tok->token_capacity = new_capacity;
	return (1);
}

int	init_tokenizer(t_tokenizer *tok)
{
	tok->i = 0;
	tok->j = 0;
	tok->type = T_WORD;
	tok->should_expand = 1;
	tok->in_quotes = 0;
	tok->quoted = 0;
	tok->quote_char = 0;
	tok->heredoc_next = 0;
	tok->token_capacity = 64;
	tok->token = malloc(tok->token_capacity);
	if (!tok->token)
		return (0);
	return (1);
}

void	tok_reset(t_tokenizer *tok)
{
	tok->j = 0;
	tok->type = T_WORD;
	tok->should_expand = 1;
	tok->quoted = 0;
}
