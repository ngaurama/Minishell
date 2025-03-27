/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 02:05:19 by npbk              #+#    #+#             */
/*   Updated: 2025/03/26 23:02:31 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append_char_to_token(t_tokenizer *tok, char c)
{
	if (!ensure_token_capacity(tok, 1))
		return ;
	tok->token[tok->j++] = c;
}

void	append_str_to_token(t_tokenizer *tok, char *str)
{
	int	i;

	if (!ensure_token_capacity(tok, ft_strlen(str)))
		return ;
	i = 0;
	while (str[i])
		tok->token[tok->j++] = str[i++];
}

int		is_space_or_meta(char c)
{
	return (
		c == ' ' ||
		c == '|' ||
		c == '<' ||
		c == '>'
	);
}

int		skip_whitespace(char *input, t_tokenizer *tok)
{
	while (input[tok->i] == ' ' || input[tok->i] == '\t')
		tok->i++;
	if (!input[tok->i])
	{
		tok->token[0] = '\0';
		return (1);
	}
	return (0);
}

void	print_parse_error(const char *token)
{
	if (token)
	{
		write(2, "minishell: parse error near unexpected token `", 46);
		write(2, token, ft_strlen(token));
		write(2, "`\n", 2);
	}
	else
	{
		write(2, "minishell: parse error near unexpected token `\\n`\n", 51);
	}
}
