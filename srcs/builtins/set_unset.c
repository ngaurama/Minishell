/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 00:14:49 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/24 01:38:07 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			is_valid_identifier(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*make_env_var(const char *key, const char *value)
{
	size_t	size;
	char	*var;

	size = ft_strlen(key) + ft_strlen(value) + 2;
	var = malloc(size);
	if (!var)
		return (NULL);
	ft_strlcpy(var, key, size);
	ft_strlcat(var, "=", size);
	ft_strlcat(var, value, size);
	return (var);
}

static int	find_env_index(t_shell *shell, const char *key)
{
	int		i;
	int		len;

	len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, len) == 0
			&& shell->env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void		set_env_var(t_shell *shell, const char *key, const char *value)
{
	char	*new_var;
	int		i;

	new_var = make_env_var(key, value);
	if (!new_var)
		return;
	i = find_env_index(shell, key);
	if (i >= 0)
	{
		free(shell->env[i]);
		shell->env[i] = new_var;
		return;
	}
	i = 0;
	while (shell->env[i])
		i++;
	if (i >= MAX_ARGS - 2)
		return (free(new_var), (void)0);
	shell->env[i] = new_var;
	shell->env[i + 1] = NULL;
}
