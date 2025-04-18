/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 02:05:19 by npbk              #+#    #+#             */
/*   Updated: 2025/04/04 13:51:34 by npagnon          ###   ########.fr       */
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

int	is_space_or_meta(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>' || c == '\t');
}

int	skip_whitespace(char *input, t_tokenizer *tok)
{
	while (input[tok->i] == ' ' || (input[tok->i] >= 9 && \
		input[tok->i] <= 13))
	{
		tok->i++;
	}
	if (!input[tok->i])
	{
		tok->token[0] = '\0';
		return (1);
	}
	return (0);
}

void	print_parse_error(const char *token)
{
	if (ft_strcmp(token, "\n") != 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putstr_fd((char *)token, STDERR_FILENO);
		write(STDERR_FILENO, "'\n", 2);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near", STDERR_FILENO);
		ft_putstr_fd("unexpected token `newline'\n", STDERR_FILENO);
	}
}
