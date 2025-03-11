/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 14:06:53 by ngaurama         ###   ########.fr       */
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
    if (!ft_strcmp(shell->command, "echo"))
        ft_echo(shell);
    else if (!ft_strcmp(shell->command, "exit"))
        ft_exit(shell);
    else if (!ft_strcmp(shell->command, "cd"))
        ft_cd(shell);
    else if (!ft_strcmp(shell->command, "pwd"))
        ft_pwd(shell);
    else if (!ft_strcmp(shell->command, "export"))
        ft_export(shell);
    else if (!ft_strcmp(shell->command, "unset"))
        ft_unset(shell);
    else if (!ft_strcmp(shell->command, "env"))
        ft_env(shell);
}
