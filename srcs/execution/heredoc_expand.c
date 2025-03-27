/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 00:39:48 by npbk              #+#    #+#             */
/*   Updated: 2025/03/27 01:27:12 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_dollar_sequence(const char *line, int i, char **result, t_shell *shell)
{
	char	*var_name;
	char	*value;
	char	*tmp;
	int		var_len;

	i++;
	var_name = extract_var_name(&line[i]);
	if (!var_name)
		return (i);

	var_len = ft_strlen(var_name);
	value = expand_var(var_name, shell);
	if (value)
	{
		tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(value);
	}
	free(var_name);
	return (i + var_len);
}

static int	skip_and_copy_literal(const char *line, int i, char **result)
{
	char	tmp[2];
	char	*joined;

	tmp[0] = line[i];
	tmp[1] = '\0';
	joined = ft_strjoin(*result, tmp);
	free(*result);
	*result = joined;
	return (i + 1);
}

char 		*heredoc_expand(char *line, t_shell *shell)
{
	int		i;
	char	*result;

	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && is_valid_var_start(line[i + 1]))
			i = handle_dollar_sequence(line, i, &result, shell);
		else
			i = skip_and_copy_literal(line, i, &result);
	}
	return result;
}
