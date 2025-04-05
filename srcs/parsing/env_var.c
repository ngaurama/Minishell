/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:55:08 by npbk              #+#    #+#             */
/*   Updated: 2025/04/05 01:55:55 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quoted_var(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	next;

	if (!input[tok->i + 1])
		return (handle_single_dollar(tok, 1));
	next = input[tok->i + 1];
	tok->i++;
	if (next == '?')
		return (handle_exit_expansion(tok, shell, 1));
	if (next == '$')
		return (handle_single_dollar(tok, 1));
	if (ft_isdigit(next))
		return (skip_digits(input, tok, 1));
	if (next == '"' || next == '\'')
	{
		append_char_to_token(tok, '$');
		return (1);
	}
	if (!ft_isalpha(next) && next != '_')
		return (handle_single_dollar(tok, 1));
	expand_variable(input, tok, shell, 0);
	return (1);
}

int	handle_quote_state(char *input, t_tokenizer *tok, int *in_quotes,
		char *quote_char)
{
	if (!*in_quotes && (input[tok->i] == '"' || input[tok->i] == '\''))
	{
		*in_quotes = 1;
		*quote_char = input[tok->i];
		tok->quoted = 1;
		tok->i++;
		return (1);
	}
	if (*in_quotes && input[tok->i] == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = 0;
		tok->i++;
		return (1);
	}
	return (0);
}

int	handle_dollar(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	next;

	if (input[tok->i] != '$')
		return (0);
	next = input[tok->i + 1];
	if (next == '\0')
		return (handle_single_dollar(tok, 0));
	if (next == '?')
		return (handle_exit_expansion(tok, shell, 0));
	if (ft_isdigit(next))
		return (skip_digits(input, tok, 0));
	if (next == '"' || next == '\'')
		return (handle_dollar_quote(input, tok, shell));
	if (is_space_or_meta(next) || (!ft_isalpha(next) && next != '_'))
		return (handle_literal_dollar(tok));
	expand_variable(input, tok, shell, 1);
	return (1);
}

void	expand_variable(char *input, t_tokenizer *tok, t_shell *shell, int skip)
{
	char	*var;
	char	*val;
	int		var_len;

	if (skip)
		tok->i++;
	var = extract_var_name(&input[tok->i]);
	if (!var)
		return ;
	var_len = ft_strlen(var);
	val = expand_var(var, shell);
	if (!val)
		val = ft_strdup("");
	if (*val == '\0' && !tok->in_quotes)
	{
		tok->i += var_len;
		free(var);
		free(val);
		return ;
	}
	append_str_to_token(tok, val);
	tok->i += var_len;
	free(var);
	free(val);
}
