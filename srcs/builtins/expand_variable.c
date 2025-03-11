/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 19:21:45 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 19:42:11 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variables(t_shell *shell, const char *input)
{
	const char	*ptr;
	char		*result;

	result = strdup("");
	ptr = input;
	if (!result)
		return (NULL);
	while (*ptr)
	{
		if (*ptr == '$')
			result = handle_variable_expansion(shell, result, &ptr);
		else
			result = handle_normal_text(result, &ptr);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*handle_variable_expansion(t_shell *shell, char *result,
		const char **ptr)
{
	char	*var_name;
	char	*var_value;

	(*ptr)++;
	var_name = extract_variable_name(ptr);
	if (!var_name)
		return (result);
	var_value = ft_getenv(shell, var_name);
	free(var_name);
	if (var_value)
		return (append_to_result(result, var_value));
	return (result);
}

char	*handle_normal_text(char *result, const char **ptr)
{
	char	temp[2] = {*(*ptr), '\0'};

	(*ptr)++;
	return (append_to_result(result, temp));
}

char	*extract_variable_name(const char **ptr)
{
	const char	*start = *ptr;
	size_t		len;
	char		*var_name;

	while (**ptr && (ft_isalnum(**ptr) || **ptr == '_'))
		(*ptr)++;
	len = *ptr - start;
	if (len == 0)
		return (NULL);
	var_name = (char *)malloc(len + 1);
	if (!var_name)
		return (NULL);
	strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

char	*append_to_result(char *result, const char *str)
{
	size_t	new_len;
	char	*new_result;

	new_len = strlen(result) + strlen(str) + 1;
	new_result = (char *)malloc(new_len);
	if (!new_result)
	{
		free(result);
		return (NULL);
	}
	strcpy(new_result, result);
	strcat(new_result, str);
	free(result);
	return (new_result);
}
