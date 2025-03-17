/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:55:57 by npbk              #+#    #+#             */
/*   Updated: 2025/03/17 19:39:48 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quotes(char *input, t_tokenizer *tok)
{
	char	quote;

	quote = input[tok->i++];
	if (quote == '"')
		tok->should_expand = 1;
	else
		tok->should_expand = 0;
	while (input[tok->i] && input[tok->i] != quote)
		tok->token[tok->j++] = input[tok->i++];
	if (input[tok->i] == quote)
		tok->i++;
	return tok->i;
}


int	handle_special(char *input, t_tokenizer *tok)
{
	if (!input[tok->i])
		return (tok->i);
	tok->token[tok->j++] = input[tok->i];
	if (input[tok->i + 1] && ((input[tok->i] == '<' &&
		 input[tok->i + 1] == '<') || (input[tok->i] == '>' &&
			 input[tok->i + 1] == '>')))
		tok->token[tok->j++] = input[++tok->i];
	tok->token[tok->j] = '\0';
	if (tok->token[0] == '|')
		tok->type = T_PIPE;
	else if (tok->token[0] == '<' && tok->token[1] == '<')
		tok->type = T_HEREDOC;
	else if (tok->token[0] == '>' && tok->token[1] == '>')
		tok->type = T_APPEND;
	else if (tok->token[0] == '<')
		tok->type = T_REDIRECT_IN;
	else if (tok->token[0] == '>')
		tok->type = T_REDIRECT_OUT;
	return (tok->i + 1);
}

int	handle_word(char *input, t_tokenizer *tok)
{
	while (input[tok->i] && input[tok->i] != ' ' && input[tok->i] != '|' &&
		input[tok->i] != '<' && input[tok->i] != '>' &&
		input[tok->i] != '\'' && input[tok->i] != '"')
	{
		tok->token[tok->j++] = input[tok->i++];
	}
	return (tok->i);
}

void	parse_next_token(char *input, t_tokenizer *tok)
{
	while (input[tok->i] == ' ' || input[tok->i] == '\t')
		tok->i++;
	if (!input[tok->i])
		return ;
	if (input[tok->i] == '\'' || input[tok->i] == '"')
		tok->i = handle_quotes(input, tok);
	else if (input[tok->i] == '|' || input[tok->i] == '<'
		 || input[tok->i] == '>')
		tok->i = handle_special(input, tok);
	else
		tok->i = handle_word(input, tok);
	tok->token[tok->j] = '\0';
}

t_arg	*tokenize_input(char *input, t_shell *shell)
{
	t_arg		*head;
	t_tokenizer	tok;
	char		*expanded_tilde;
	char		*expanded_var;

	tok.i = 0;
	head = NULL;
	while (input[tok.i])
	{
		tok.j = 0;
		tok.type = T_WORD;
		tok.should_expand = 1;
		parse_next_token(input, &tok);
		if (!tok.token[0])
			continue;
		expand_token(&tok, shell, &expanded_tilde, &expanded_var);
		if (!expanded_var)
			return NULL;
		head = add_token(head, expanded_var, tok.type);
		if (expanded_var != expanded_tilde)
			free(expanded_var);
		if (expanded_tilde != tok.token)
			free(expanded_tilde);
	}
	return (head);
}
