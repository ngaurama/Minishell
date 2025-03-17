/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:55:57 by npbk              #+#    #+#             */
/*   Updated: 2025/03/16 14:34:31 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_quotes(char *input, int i, char *token, int *j)
{
	char	quote;

	quote = input[i++];
	while (input[i] && input[i] != quote)
		token[(*j)++] = input[i++];
	if (input[i] == quote)
		i++;
	return (i);
}

int	handle_special(char *input, int i, char *token, int *j, int *type)
{
	token[(*j)++] = input[i++];
	if ((token[0] == '<' && input[i] == '<') || (token[0] == '>'
			&& input[i] == '>'))
		token[(*j)++] = input[i++];
	if (token[0] == '|')
		*type = T_PIPE;
	else if (token[0] == '<' && token[1] == '<')
		*type = T_HEREDOC;
	else if (token[0] == '>' && token[1] == '>')
		*type = T_APPEND;
	else if (token[0] == '<')
		*type = T_REDIRECT_IN;
	else if (token[0] == '>')
		*type = T_REDIRECT_OUT;

	return (i + 1);
}

int	handle_word(char *input, int i, char *token, int *j)
{
	while (input[i] && input[i] != ' ' && input[i] != '|' && input[i] != '<'
		&& input[i] != '>' && input[i] != '\'' && input[i] != '"')
		token[(*j)++] = input[i++];
	return (i);
}

void	parse_next_token(char *input, int *i, char *token, int *j, int *type)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (!input[*i])
		return ;
	if (input[*i] == '\'' || input[*i] == '"')
		*i = handle_quotes(input, *i, token, j);
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		*i = handle_special(input, *i, token, j, type);
	else
		*i = handle_word(input, *i, token, j);
	token[*j] = '\0';
}

t_arg	*tokenize_input(char *input, t_shell *shell)
{
	t_arg			*head;
	t_tokenizer		tok;
	char			*expanded_tilde;
	char			*expanded_var;

	tok.i = 0;
	head = NULL;
	while (input[tok.i])
	{
		tok.j = 0;
		tok.type = T_WORD;
		parse_next_token(input, &tok.i, tok.token, &tok.j, &tok.type);
		if (!tok.token[0])
			continue;
		expand_token(tok.token, shell, &expanded_tilde, &expanded_var);
		if (!expanded_var)
			return (NULL);
		head = add_token(head, expanded_var, tok.type);
		if (expanded_var != expanded_tilde)
			free(expanded_var);
		if (expanded_tilde != tok.token)
			free(expanded_tilde);
	}
	return (head);
}
