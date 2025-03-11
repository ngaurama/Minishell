/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 14:08:54 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_echo(t_shell *shell)
{
    t_arg *current = shell->arguments->next;
    int n_flag = 0;

    if (current && ft_strcmp(current->value, "-n") == 0)
    {
        n_flag = 1;
        current = current->next;
    }
    while (current)
    {
        char *expanded = expand_variables(shell, current->value);
        if (expanded)
        {
            printf("%s", expanded);
            free(expanded);
        }

        if (current->next)
            printf(" ");
        current = current->next;
    }
    if (!n_flag)
        printf("\n");
}
