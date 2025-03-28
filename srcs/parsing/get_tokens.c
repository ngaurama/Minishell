/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:55:57 by npbk              #+#    #+#             */
/*   Updated: 2025/03/28 18:29:15 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quotes(char *input, t_tokenizer *tok,
			int in_word, t_shell *shell)
{
	char	quote;

	quote = input[tok->i++];
	tok->should_expand = (quote == '"');
	tok->in_quotes = 1;
	tok->quoted = 1;
	while (input[tok->i] && input[tok->i] != quote)
	{
		if (input[tok->i] == '$' && quote == '"')
		{
			if (handle_quoted_var(input, tok, shell))
				continue ;
		}
		else
			append_char_to_token(tok, input[tok->i++]);
	}
	if (input[tok->i] == quote)
	{
		tok->i++;
		tok->in_quotes = 0;
	}
	if (!in_word)
		tok->token[tok->j] = '\0';
	return (tok->i);
}

int	handle_special(char *input, t_tokenizer *tok)
{
	if (!input[tok->i])
		return (tok->i);
	append_char_to_token(tok, input[tok->i]);
	if (input[tok->i + 1] && ((input[tok->i] == '<' && \
		input[tok->i + 1] == '<') || (input[tok->i] == '>' && \
			input[tok->i + 1] == '>')))
		append_char_to_token(tok, input[++tok->i]);
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

int	handle_word(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	current;

	tok->in_quotes = 0;
	tok->quote_char = 0;
	while (input[tok->i]
		&& (tok->in_quotes || !is_space_or_meta(input[tok->i])))
	{
		current = input[tok->i];
		if (handle_quote_state(input, tok, &tok->in_quotes, &tok->quote_char))
			continue ;
		if (!tok->in_quotes && tok->j == 0 && current == '~' && \
			handle_tilde(input, tok, shell))
			continue ;
		if (current == '$' && (!tok->in_quotes || tok->quote_char == '"') && \
			handle_dollar(input, tok, shell))
			continue ;
		append_char_to_token(tok, current);
		tok->i++;
	}
	if (tok->j == 0)
	tok->token[0] = '\0';
	tok->token[tok->j] = '\0';
	return (tok->i);
}

void	parse_next_token(char *input, t_tokenizer *tok, t_shell *shell)
{
	if (skip_whitespace(input, tok))
		return ;
	if (tok->heredoc_next && tok->quoted == 0)
		tok->should_expand = 0;
	if (input[tok->i] == '$' && (input[tok->i + 1] == '"'
			|| input[tok->i + 1] == '\''))
	{
		append_char_to_token(tok, '$');
		tok->i++;
	}
	if (input[tok->i] == '\'' || input[tok->i] == '"')
	{
		tok->i = handle_quotes(input, tok, 0, shell);
		if (input[tok->i] && !is_space_or_meta(input[tok->i]))
			tok->i = handle_word(input, tok, shell);
	}
	else if (input[tok->i] == '|' || input[tok->i] == '<'
		|| input[tok->i] == '>')
		tok->i = handle_special(input, tok);
	else
		tok->i = handle_word(input, tok, shell);
	tok->token[tok->j] = '\0';
	if (tok->token[0] == '\0' && !tok->quoted)
		return ;
	tok->heredoc_next = (tok->type == T_HEREDOC);
}

t_arg	*tokenize_input(char *input, t_shell *shell)
{
	t_arg		*head;
	t_tokenizer	tok;

	head = NULL;
	if (!init_tokenizer(&tok))
		return (NULL);
	while (input[tok.i])
	{
		tok_reset(&tok);
		parse_next_token(input, &tok, shell);
		if (!tok.token[0])
		{
			head = add_token(head, "", tok.type, tok.quoted);
			continue ;
		}
		head = add_token(head, tok.token, tok.type, tok.quoted);
	}
	free_tokenizer(&tok);
	return (head);
}
