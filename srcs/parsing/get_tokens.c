/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:55:57 by npbk              #+#    #+#             */
/*   Updated: 2025/03/24 01:37:15 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		handle_quotes(char *input, t_tokenizer *tok, int in_word, t_shell *shell)
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
				continue;
		}
		else
			tok->token[tok->j++] = input[tok->i++];
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

int		handle_special(char *input, t_tokenizer *tok)
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

int		handle_word(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*var;
	char	*val;
	
	while (input[tok->i] && !is_space_or_meta(input[tok->i]))
	{
		if (input[tok->i] == '\'' || input[tok->i] == '"')
			tok->i = handle_quotes(input, tok, 1, shell);
		else if (input[tok->i] == '$' && tok->should_expand)
		{
			tok->i++;
			var = extract_var_name(&input[tok->i]);
			if (!var)
				continue;
			tok->i += ft_strlen(var);
			val = expand_var(var, shell);
			if (val)
				append_str_to_token(tok, val);
			free(var);
			free(val);
		}
		else
			tok->token[tok->j++] = input[tok->i++];
	}
	tok->token[tok->j] = '\0';
	return (tok->i);
}

void	parse_next_token(char *input, t_tokenizer *tok, t_shell *shell)
{
	while (input[tok->i] == ' ' || input[tok->i] == '\t')
		tok->i++;
	if (!input[tok->i])
	{
		tok->token[0] = '\0';
		return ;
	}
	if (input[tok->i] == '\'' || input[tok->i] == '"')
	{
		tok->i = handle_quotes(input, tok, 0, shell);
		if (input[tok->i] && input[tok->i] != ' ' && input[tok->i] != '|' &&
			input[tok->i] != '<' && input[tok->i] != '>')
			tok->i = handle_word(input, tok, shell);
	}
	else if (input[tok->i] == '|' || input[tok->i] == '<'
		 || input[tok->i] == '>')
		tok->i = handle_special(input, tok);
	else
		tok->i = handle_word(input, tok, shell);
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
	tok.token = malloc(ft_strlen(input) + 1);
	if (!tok.token)
		return (NULL);
	while (input[tok.i])
	{
		tok_reset(&tok);
		parse_next_token(input, &tok, shell);
		if (!tok.token[0])
		{
			head = add_token(head, "", tok.type);
			continue;
		}
		expand_token(&tok, shell, &expanded_tilde, &expanded_var);
		head = add_token(head, expanded_var, tok.type);
		free_expanded_tokens(expanded_tilde, expanded_var, &tok);
	}
	free(tok.token);
	return (head);
}
