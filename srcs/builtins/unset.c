/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:53 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/24 01:43:34 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	unset_env_var(t_shell *shell, const char *key)
{
	int i;
	int len;

	i = 0;
	len = ft_strlen(key);
	while(shell->env[i])
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
			return;
		}
		i++;
	}
}

void		ft_unset(t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (shell->cmds->args[i])
	{
		if (!is_valid_identifier(shell->cmds->args[i]))
		{
			printf("unset: '%s': invalid parameter name\n",
				shell->cmds->args[i]);
			status = 1;
		}
		else
			unset_env_var(shell, shell->cmds->args[i]);
		i++;
	}
	shell->exit_status = status;
}
