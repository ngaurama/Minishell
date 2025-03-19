/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:18:39 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_exit(t_shell *shell)
{
    int status = 0;

    if (shell->cmds->args[1])
        status = ft_atoi(shell->cmds->args[1]);

    free_shell(shell);
    exit(status);
}

void execute_built_in(t_shell *shell)
{
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
    {
        perror("dup failed");
        return;
    }
    if (redirection(shell) != 0)
    {
        close(saved_stdin);
        close(saved_stdout);
        return;
    }
    if (ft_strcmp(shell->cmds->args[0], "echo") == 0)
        ft_echo(shell);
    else if (ft_strcmp(shell->cmds->args[0], "cd") == 0)
        ft_cd(shell);
    else if (ft_strcmp(shell->cmds->args[0], "pwd") == 0)
        ft_pwd(shell);
    else if (ft_strcmp(shell->cmds->args[0], "export") == 0)
        ft_export(shell);
    else if (ft_strcmp(shell->cmds->args[0], "unset") == 0)
        ft_unset(shell);
    else if (ft_strcmp(shell->cmds->args[0], "env") == 0)
        ft_env(shell);
    else if (ft_strcmp(shell->cmds->args[0], "exit") == 0)
        ft_exit(shell);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}
