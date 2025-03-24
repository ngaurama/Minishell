/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/23 20:39:50 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void update_exit_status(t_shell *shell, int status)
{
    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
    else
        shell->exit_status = 1;
}

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

int save_fds(int *saved_stdin, int *saved_stdout)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
    {
        perror("dup failed");
        return (0);
    }
    return (1);
}

void restore_fds(int saved_stdin, int saved_stdout)
{
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
}

void handle_command_not_found(t_shell *shell)
{
    write(2, "minishell: command not found: ", 30);
    write(2, shell->cmds->args[0], ft_strlen(shell->cmds->args[0]));
    write(2, "\n", 1);
	shell->exit_status = 127;
}

void execute_child_process(t_shell *shell)
{
    if (execve(shell->full_path, shell->cmds->args, shell->env) == -1)
    {
        perror("execve failed");
        exit(1);
    }
}

int execute_command(t_shell *shell)
{
    if (!shell || !shell->cmds || !shell->cmds->args[0])
        return (1);

    int saved_stdin, saved_stdout;
    if (!save_fds(&saved_stdin, &saved_stdout))
        return (1);
    if (redirection(shell) != 0)
        return (restore_fds(saved_stdin, saved_stdout), 1);
    if (find_full_path(shell, shell->cmds->args[0]) != 0)
        return (handle_command_not_found(shell), restore_fds(saved_stdin, saved_stdout), 1);
    pid_t pid = fork();
    if (pid == 0)
        execute_child_process(shell);
    else if (pid > 0)
        shell->pid = pid;
    else
    {
        perror("fork failed");
        restore_fds(saved_stdin, saved_stdout);
        return (1);
    }
    restore_fds(saved_stdin, saved_stdout);
    return (0);
}