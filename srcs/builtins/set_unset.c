/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 00:14:49 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:34:20 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void set_env_var(t_shell *shell, const char *key, const char *value)
{
    int i = 0;
    int len = ft_strlen(key);
    char *new_var;

    new_var = malloc(len + ft_strlen(value) + 2);
    if (!new_var)
        return;
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);

    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], key, len) == 0 && shell->env[i][len] == '=')
        {
            free(shell->env[i]);
            shell->env[i] = new_var;
            return;
        }
        i++;
    }
    if (i >= MAX_ARGS - 1)
    {
        free(new_var);
        return;
    }
    shell->env[i] = new_var;
    shell->env[i + 1] = NULL;
}

void unset_env_var(t_shell *shell, const char *key)
{
    int i = 0;
    int len = ft_strlen(key);

    while(shell->env[i])
    {
        if (ft_strncmp(shell->env[i], key, len) == 0 && shell->env[i][len] == '=')
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
