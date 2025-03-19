/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:30:09 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_export(t_shell *shell)
{
    int i = 1;

    if (!shell->cmds->args[i])
    {
		i = 0;
        while (shell->env[++i])
		{
            printf("declare -x %s\n", shell->env[i]);
			i++;
		}
        return;
    }
    while (shell->cmds->args[i])
    {
        char *key = shell->cmds->args[i];
        char *value = ft_strchr(key, '=');
        if (value)
        {
            *value = '\0';
            value++;
            set_env_var(shell, key, value);
        }
        i++;
    }
}
