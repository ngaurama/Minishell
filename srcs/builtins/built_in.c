/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/21 17:19:54 by ngaurama         ###   ########.fr       */
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

int save_and_redirect_fds(int *saved_stdin, int *saved_stdout, t_shell *shell)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
    {
        perror("dup failed");
        return (1);
    }
    if (redirection(shell) != 0)
    {
        close(*saved_stdin);
        close(*saved_stdout);
        return (1);
    }
    return (0);
}

int check_built_in(t_command *cmd)
{
    int i = 0;
    if (!cmd || !cmd->args[0])
        return (0);
    const char *command = cmd->args[0];
    const char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};
    while(builtins[i])
    {
        if (ft_strcmp(command, builtins[i]) == 0)
            return (1);
        i++;
    }
    return (0);
}

void execute_built_in(t_shell *shell)
{
    int saved_stdin, saved_stdout;

    if (save_and_redirect_fds(&saved_stdin, &saved_stdout, shell) != 0)
        return;
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

    restore_fds(saved_stdin, saved_stdout);
}
