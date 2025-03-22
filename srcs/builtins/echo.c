/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/21 14:26:11 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_echo(t_shell *shell)
{
    int i = 1;
    int newline = 1;

    if (shell->cmds->args[i] && ft_strcmp(shell->cmds->args[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    while (shell->cmds->args[i])
    {
        printf("%s", shell->cmds->args[i]);
        if (shell->cmds->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}
