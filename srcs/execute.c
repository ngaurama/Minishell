/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:18:30 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int find_full_path(t_shell *shell, const char *command)
{
    char *path_var;
    char *path_var_copy;
    char *path;

    if (!command)
        return (1);
    path_var = getenv("PATH");
    if (!path_var)
        return (1);
    path_var_copy = ft_strdup(path_var);
    if (!path_var_copy)
        return (1);
    path = ft_strtok(path_var_copy, ":");
    if (shell->full_path)
    {
        free(shell->full_path);
        shell->full_path = NULL;
    }
    while (path)
    {
        shell->full_path = malloc(ft_strlen(path) + ft_strlen(command) + 2);
        if (!shell->full_path)
        {
            free(path_var_copy);
            return (1);
        }
        ft_strcpy(shell->full_path, path);
        ft_strcat(shell->full_path, "/");
        ft_strcat(shell->full_path, command);
        if (access(shell->full_path, F_OK | X_OK) == 0)
        {
            free(path_var_copy); 
            return (0); 
        }
        free(shell->full_path);
        shell->full_path = NULL;
        path = ft_strtok(NULL, ":");
    }
    free(path_var_copy);
    return (1);
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

int execute_command(t_shell *shell)
{
    if (!shell || !shell->cmds || !shell->cmds->args[0])
        return (1);
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
    {
        perror("dup failed");
        return (1);
    }
    if (redirection(shell) != 0)
    {
        close(saved_stdin);
        close(saved_stdout);
        return (1);
    }
    if (check_built_in(shell->cmds))
    {
        execute_built_in(shell);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return (0);
    }
    if (find_full_path(shell, shell->cmds->args[0]) != 0)
    {
        write(2, "minishell: command not found: ", 30);
        write(2, shell->cmds->args[0], ft_strlen(shell->cmds->args[0]));
        write(2, "\n", 1);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return (1);
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        if (execve(shell->full_path, shell->cmds->args, shell->env) == -1)
        {
            perror("execve failed");
            exit(1);
        }
    }
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }
    else
    {
        perror("fork failed");
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
        return (1);
    }
    return (0);
}