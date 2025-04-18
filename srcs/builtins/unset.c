/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:53 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 13:52:54 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	unset_env_var(t_shell *shell, const char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, len) == 0
			&& shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			while (shell->env[i + 1])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			shell->env[i] = NULL;
			return ;
		}
		i++;
	}
}

void	ft_unset(t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (shell->cmds->args[i])
	{
		unset_env_var(shell, shell->cmds->args[i]);
		i++;
	}
	shell->exit_status = status;
}
