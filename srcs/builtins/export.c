/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 19:25:29 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_export(t_shell *shell)
{
	t_arg	*current;
	char	*key_value;
	char	*equal_sign;
	char	*key;
	char	*value;
	int		found;
	int		i;
	int		env_count;
	char	**new_env;

	current = shell->arguments->next;
	while (current)
	{
		key_value = current->value;
		equal_sign = ft_strchr(key_value, '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = key_value;
			value = equal_sign + 1;
			found = 0;
			i = 0;
			while (shell->env[i])
			{
				if (ft_strncmp(shell->env[i], key, ft_strlen(key)) == 0
					&& shell->env[i][ft_strlen(key)] == '=')
				{
					free(shell->env[i]);
					shell->env[i] = malloc(ft_strlen(key) + ft_strlen(value)
							+ 2);
					if (!shell->env[i])
						return ;
					ft_strcpy(shell->env[i], key);
					ft_strlcat(shell->env[i], "=", ft_strlen(key) + 2);
					ft_strlcat(shell->env[i], value, ft_strlen(key)
						+ ft_strlen(value) + 2);
					found = 1;
					break ;
				}
				i++;
			}
			if (!found)
			{
				env_count = 0;
				while (shell->env[env_count])
					env_count++;
				new_env = malloc((env_count + 2) * sizeof(char *));
				if (!new_env)
					return ;
				i = -1;
				while (i++ < env_count)
					new_env[i] = shell->env[i];
				new_env[env_count] = malloc(ft_strlen(key) + ft_strlen(value)
						+ 2);
				if (!new_env[env_count])
				{
					free(new_env);
					return ;
				}
				ft_strcpy(new_env[env_count], key);
				ft_strlcat(new_env[env_count], "=", ft_strlen(key) + 2);
				ft_strlcat(new_env[env_count], value, ft_strlen(key)
					+ ft_strlen(value) + 2);
				new_env[env_count + 1] = NULL;
				free(shell->env);
				shell->env = new_env;
			}
		}
		current = current->next;
	}
}
