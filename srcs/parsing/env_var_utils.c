/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:03:53 by npbk              #+#    #+#             */
/*   Updated: 2025/03/15 19:11:18 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(char **env, char *var_name)
{
	int		i;
	int		len;
	char	*env_var;

	i = 0;
	len = ft_strlen(var_name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, len) && env[i][len] == '=')
		{
			env_var = ft_strdup(env[i] + len + 1);
			return (env_var);
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*extract_var_name(char *start)
{
	int		len;
	char	*var_name;

	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
		len++;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	len = 0;
	while (start[len] && (ft_isalnum(start[len]) || start[len] == '_'))
	{
		var_name[len] = start[len];
		len++;
	}
	var_name[len] = '\0';
	return (var_name);
}

char	*create_expanded_token(char *token, char *var_start,
	char *var_value, char *var_name)
{
	char	*expanded;
	int		prefix_len;
	int		total_size;

	prefix_len = var_start - token;
	total_size = prefix_len + ft_strlen(var_value) + ft_strlen(var_start) + 1;
	expanded = malloc(total_size);
	if (!expanded)
		return (NULL);
	ft_strlcpy(expanded, token, prefix_len + 1);
	ft_strlcat(expanded, var_value, total_size);
	ft_strlcat(expanded, var_start + ft_strlen(var_name) + 1, total_size);
	return (expanded);
}
