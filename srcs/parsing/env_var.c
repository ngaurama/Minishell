/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:55:08 by npbk              #+#    #+#             */
/*   Updated: 2025/03/28 18:25:04 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quoted_var(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*var;
	char	*val;
	int		var_len;

	tok->i++;
	var = extract_var_name(&input[tok->i]);
	if (!var || var[0] == '\0')
	{
		append_char_to_token(tok, '$');
		free(var);
		return (0);
	}
	var_len = ft_strlen(var);
	val = expand_var(var, shell);
	if (val)
		append_str_to_token(tok, val);
	tok->i += var_len;
	free(var);
	free(val);
	return (1);
}

int	handle_tilde(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*home;

	if (tok->j == 0 && input[tok->i] == '~')
	{
		home = get_env_value(shell->env, "HOME");
		if (home)
			append_str_to_token(tok, home);
		free(home);
		tok->i++;
		return (1);
	}
	return (0);
}

int	handle_quote_state(char *input, t_tokenizer *tok,
			int *in_quotes, char *quote_char)
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
	if (!tok->should_expand)
	{
		append_char_to_token(tok, '$');
		tok->i++;
		return (1);
	}
	next = input[tok->i + 1];
	if (!should_expand_dollar(next, tok->in_quotes, tok->quote_char))
	{
		append_char_to_token(tok, '$');
		tok->i++;
	}
	else
		expand_variable(input, tok, shell);
	return (1);
}

void	expand_variable(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*var;
	char	*val;
	int		var_len;

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
