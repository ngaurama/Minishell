/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 11:35:09 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int find_full_path(t_shell *shell)
{
    char *path_var = getenv("PATH");
    if (!path_var)
        return 1;
    char *path = ft_strtok(path_var, ":");
    while (path)
    {
        shell->full_path = malloc(strlen(path) + strlen(shell->command) + 2);
        if (!shell->full_path)
            return 1;
        strcpy(shell->full_path, path);
        strcat(shell->full_path, "/");
        strcat(shell->full_path, shell->command);
        if (access(shell->full_path, F_OK) == 0)
            return 0;
        free(shell->full_path);
        shell->full_path = NULL;
        path = ft_strtok(NULL, ":");
    }
    return 1;
}

int check_built_in(t_shell *shell)
{
    int result;

    result = 1;
    if (strcmp(shell->command, "echo") || strcmp(shell->command, "exit") || strcmp(shell->command, "cd") || 
        strcmp(shell->command, "pwd") || strcmp(shell->command, "export") || strcmp(shell->command, "unset") ||
        strcmp(shell->command, "env"))
        result = 1;
    else
        result = 0;
    return (result);
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
    pid_t pid = fork();
    if (pid == 0)
    {
        char **args = malloc(sizeof(char *) * (count_arguments(shell) + 1));

        t_arg *temp = shell->arguments;
        int i = 0;
        while (temp)
        {
            args[i++] = temp->value;
            temp = temp->next;
        }
        args[i] = NULL;

        if (execve(shell->full_path, args, shell->env) == -1)
        {
            perror("execve failed");
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
