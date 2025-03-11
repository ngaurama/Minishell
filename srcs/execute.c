/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 14:22:47 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int find_full_path(t_shell *shell)
{
    char *path_var = getenv("PATH");
    char *path_var_copy;
    
    if (!path_var)
        return (1);
    path_var_copy = ft_strdup(path_var);
    if (!path_var_copy)
        return (1);
    char *path = ft_strtok(path_var_copy, ":");
    if (shell->full_path)
    {
        free(shell->full_path);
        shell->full_path = NULL;
    }
    while (path)
    {
        shell->full_path = malloc(ft_strlen(path) + ft_strlen(shell->command) + 2);
        if (!shell->full_path)
            return 1;
        ft_strcpy(shell->full_path, path);
        ft_strlcat(shell->full_path, "/", sizeof(shell->full_path));
        ft_strlcat(shell->full_path, shell->command, sizeof(shell->full_path));
        if (access(shell->full_path, F_OK) == 0)
            return (free(path_var_copy), 0);
        free(shell->full_path);
        shell->full_path = NULL;
        path = ft_strtok(NULL, ":");
    }   
    return (free(path_var_copy), 1);
}

int check_built_in(t_shell *shell)
{
    if (!shell->command)
        return (0);
    if (!ft_strcmp(shell->command, "echo") || !ft_strcmp(shell->command, "exit") ||
        !ft_strcmp(shell->command, "cd") || !ft_strcmp(shell->command, "pwd") ||
        !ft_strcmp(shell->command, "export") || !ft_strcmp(shell->command, "unset") ||
        !ft_strcmp(shell->command, "env"))
        return (1);
    return (0);
}

int count_arguments(t_shell *shell)
{
    int count = 0;
    t_arg *temp = shell->arguments;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

int execute_command(t_shell *shell)
{
    pid_t pid;
    char **args;
    t_arg *temp;
    int i;
    
    args = malloc(sizeof(char *) *(count_arguments(shell) + 1));
    if (!args)
        exit (1);
    temp = shell->arguments;
    i = 0;
    while (temp)
    {
        args[i++] = temp->value;
        temp = temp->next;
    }
    args[i] = NULL;
    pid = fork();
    if (pid == 0)
    {
        if (execve(shell->full_path, args, shell->env) == -1)
        {
            perror("execve failed");
            free(args);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
    }
    else
    {
        perror("fork failed");
    }
    return 0;
}
