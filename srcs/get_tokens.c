/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 13:55:57 by npbk              #+#    #+#             */
/*   Updated: 2025/03/13 18:08:19 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		handle_quotes(char *input, int i, char *token, int *j)
{
	char quote;
	
	quote = input[i++];
	while (input[i] && input[i] != quote)
		token[(*j)++] = input[i++];
	if (input[i] == quote)
		i++;
	return (i);
}

int	handle_special(char *input, int i, char *token, int *j, int *type)
{
	if (!input[i])
		return (i);
	token[(*j)++] = input[i];
	if (input[i + 1] && ((input[i] == '<' && input[i + 1] == '<') ||
		(input[i] == '>' && input[i + 1] == '>')))
		token[(*j)++] = input[++i];
	token[*j] = '\0';
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

int		handle_word(char *input, int i, char *token, int *j)
{
	while (input[i] && input[i] != ' ' && input[i] != '|' &&
		input[i] != '<' && input[i] != '>' && input[i] != '\'' 
		&& input[i] != '"')
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

t_arg	*tokenize_input(char *input)
{
	t_arg	*head;
	char	token[TOKEN_SIZE];
	int		i;
	int		j;
	int		type;

	i = 0;
	head = NULL;
	while (input[i])
	{
		j = 0;
		type = T_WORD;
		parse_next_token(input, &i, token, &j, &type);
		if (!token[0])
			continue ;
		//printf("TOKEN: [%s] (Type: %d)\n", token, type); // DEBUG
		head = add_token(head, token, type);
	}
	return (head);
}

