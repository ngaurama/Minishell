/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:11 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/17 17:35:37 by ngaurama         ###   ########.fr       */
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

int	check_built_in(t_shell *shell)
{
	if (!shell->command)
		return (0);
	if (!ft_strcmp(shell->command, "echo") || !ft_strcmp(shell->command, "exit")
		|| !ft_strcmp(shell->command, "cd") || !ft_strcmp(shell->command, "pwd")
		|| !ft_strcmp(shell->command, "export") || !ft_strcmp(shell->command,
			"unset") || !ft_strcmp(shell->command, "env") || !ft_strcmp(shell->command, "$"))
		return (1);
	return (0);
}

int execute_command(t_shell *shell)
{
    if (!shell || !shell->cmds)
    {
        write(2, "minishell: invalid shell or command\n", 36);
        return (1);
    }
    if (check_built_in(shell))
    {
        execute_built_in(shell);
        return (0);
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        redirection(shell);
        if (execve(shell->full_path, shell->cmds->args, shell->env) == -1)
        {
            perror("execve failed");
            exit(1);
        }
    }
    else if (pid > 0)
        waitpid(pid, NULL, 0);
    else
    {
        perror("fork failed");
        return (1);
    }
    return (0);
}
