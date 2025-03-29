/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 17:34:40 by npbk              #+#    #+#             */
/*   Updated: 2025/03/29 19:06:39 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_single_dollar(t_tokenizer *tok, int quoted)
{
	append_char_to_token(tok, '$');
	if (!quoted)
		tok->i++;
	return (1);
}

int	handle_exit_expansion(t_tokenizer *tok, t_shell *shell, int quoted)
{
	char	*status;
	
	status = ft_itoa(shell->exit_status);
	append_str_to_token(tok, status);
	free(status);
	if (!quoted)
		tok->i += 2;
	else
		tok->i++;
	return (1);
}

int	skip_digits(char *input, t_tokenizer *tok, int quoted)
{
	if (!quoted)
		tok->i++;
	while (ft_isdigit(input[tok->i]))
		tok->i++;
	return (1);
}

int	handle_dollar_quote(char *input, t_tokenizer *tok, t_shell *shell)
{
	tok->i++;
	tok->i = handle_quotes(input, tok, 0, shell);
	return (1);
}

int	handle_literal_dollar(t_tokenizer *tok)
{
	append_char_to_token(tok, '$');
	tok->i++;
	return (1);
}
