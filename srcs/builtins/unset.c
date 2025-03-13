/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:53 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 23:21:56 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_unset(t_shell *shell)
{
    t_arg *current = shell->arguments->next;

    while (current)
    {
        char *key = current->value;
        int i = 0;
        while (shell->env[i])
        {
            if (strncmp(shell->env[i], key, strlen(key)) == 0 && shell->env[i][strlen(key)] == '=')
            {
                free(shell->env[i]);
                int j = i;
                while (shell->env[j + 1])
                {
                    shell->env[j] = shell->env[j + 1];
                    j++;
                }
                shell->env[j] = NULL;
                break;
            }
            i++;
        }
        current = current->next;
    }
}