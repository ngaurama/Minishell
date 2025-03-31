/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 16:23:07 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_target_dir(t_shell *shell)
{
	char	*dir;

	dir = shell->cmds->args[1];
	if (!dir)
	{
		dir = get_env_value(shell->env, "HOME");
		if (!dir)
		{
			print_error("cd: HOME not set");
			return (NULL);
		}
	}
	else if (ft_strncmp(dir, "-", 2) == 0)
	{
		dir = get_env_value(shell->env, "OLDPWD");
		if (!dir)
		{
			print_error("cd: OLDPWD not set");
			return (NULL);
		}
		printf("%s\n", dir);
	}
	return (dir);
}

static int change_directory(char *dir, char *oldpwd, t_shell *shell)
{
    char *new_dir;

	if (chdir(dir) == -1)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        ft_putstr_fd(dir, STDERR_FILENO);
        ft_putstr_fd(": ", STDERR_FILENO);
        ft_putstr_fd(strerror(errno), STDERR_FILENO);
        ft_putstr_fd("\n", STDERR_FILENO);
        shell->exit_status = 1;
        return (1);
    }
    new_dir = getcwd(NULL, 0);
    if (!new_dir)
    {
        if (ft_strcmp(dir, "..") == 0)
        {
            char *tmp = ft_strjoin(oldpwd, "/..");
            new_dir = tmp;
        }
        else if (dir[0] == '/')
            new_dir = ft_strdup(dir);
        else
        {
            char *tmp = ft_strjoin(oldpwd, "/");
            new_dir = ft_strjoin(tmp, dir);
            free(tmp);
        }
        ft_putstr_fd("cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", STDERR_FILENO);
        shell->exit_status = 1;
    }
    set_env_var(shell, "OLDPWD", oldpwd);
    set_env_var(shell, "PWD", new_dir);
    free(shell->current_dir);
    shell->current_dir = new_dir;
    if (shell->exit_status == 0)
        shell->exit_status = 0;
    return (0);
}

void ft_cd(t_shell *shell)
{
    char *dir;
    char *oldpwd;

    oldpwd = getcwd(NULL, 0);
	if (!oldpwd) 
	{
		if (shell->current_dir)
			oldpwd = ft_strdup(shell->current_dir);
		else
			oldpwd = ft_strdup("/");
	}
    dir = get_target_dir(shell);
    if (!dir)
    {
        shell->exit_status = 1;
        free(oldpwd);
        return;
    }
    change_directory(dir, oldpwd, shell);
    free(oldpwd);
}
