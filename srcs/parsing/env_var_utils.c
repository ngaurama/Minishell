/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:03:53 by npbk              #+#    #+#             */
/*   Updated: 2025/03/31 12:43:26 by npagnon          ###   ########.fr       */
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
	return (NULL);
}

char	*extract_var_name(const char *str)
{
	int		len;
	char	*var;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (NULL);
	len = 1;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	var = malloc(sizeof(char) * (len + 1));
	if (!var)
		return (NULL);
	ft_strncpy(var, str, len);
	var[len] = '\0';
	return (var);
}

char	*expand_var(char *var, t_shell *shell)
{
	if (ft_strncmp(var, "?", 2) == 0)
		return (ft_itoa(shell->exit_status));
	return (get_env_value(shell->env, var));
}

int	is_valid_var_start(char c)
{
	if (ft_isalpha(c) || c == '_' || c == '?')
		return (1);
	return (0);
}
