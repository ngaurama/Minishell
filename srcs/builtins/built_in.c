/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 23:21:15 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_exit(t_shell *shell)
{
    free_shell(shell);
    exit(0);
}

void execute_built_in(t_shell *shell)
{
    if (!strcmp(shell->command, "echo"))
        ft_echo(shell);
    else if (!strcmp(shell->command, "exit"))
        ft_exit(shell);
    else if (!strcmp(shell->command, "cd"))
        ft_cd(shell);
    else if (!strcmp(shell->command, "pwd"))
        ft_pwd(shell);
    else if (!strcmp(shell->command, "export"))
        ft_export(shell);
    else if (!strcmp(shell->command, "unset"))
        ft_unset(shell);
    else if (!strcmp(shell->command, "env"))
        ft_env(shell);
}
