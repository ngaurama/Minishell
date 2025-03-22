/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 02:05:19 by npbk              #+#    #+#             */
/*   Updated: 2025/03/22 04:08:56 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append_str_to_token(t_tokenizer *tok, char *str)
{
	int i = 0;

	while (str[i])
		tok->token[tok->j++] = str[i++];
}

int	is_space_or_meta(char c)
{
	return (
		c == ' ' ||
		c == '|' ||
		c == '<' ||
		c == '>'
	);
}

void	tok_reset(t_tokenizer *tok)
{
	tok->j = 0;
	tok->type = T_WORD;
	tok->should_expand = 1;
	tok->quoted = 0;
}

int	handle_quoted_var(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*var;
	char	*val;

	tok->i++;
	var = extract_var_name(&input[tok->i]);
	if (!var || var[0] == '\0')
	{
		free(var);
		return (0);
	}
	tok->i += ft_strlen(var);
	val = expand_var(var, shell);
	if (val)
		append_str_to_token(tok, val);
	free(var);
	free(val);
	return (1);
}

char *expand_var(char *var, t_shell *shell)
{
	if (ft_strncmp(var, "?", 2) == 0)
		return ft_itoa(shell->exit_status);
	return (get_env_value(shell->env, var));
}
