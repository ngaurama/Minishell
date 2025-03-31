/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 12:35:42 by npagnon          ###   ########.fr       */
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

static int	change_directory(char *dir, char *oldpwd, t_shell *shell)
{
	char	*cwd;

	if (chdir(dir) == -1)
	{
		perror("cd");
		shell->exit_status = 1;
		return (1);
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_var(shell, "PWD", cwd);
		set_env_var(shell, "OLDPWD", oldpwd);
		free(cwd);
	}
	else
		perror("cd: getcwd after chdir");

	shell->exit_status = 0;
	return (0);
}

void	ft_cd(t_shell *shell)
{
	char	*dir;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("cd: getcwd");
		oldpwd = ft_strdup("");
	}
	dir = get_target_dir(shell);
	if (!dir)
	{
		shell->exit_status = 1;
		free(oldpwd);
		return ;
	}
	if (change_directory(dir, oldpwd, shell) && dir != shell->cmds->args[1])
		free(dir);
	else if (dir != shell->cmds->args[1])
		free(dir);
	free(oldpwd);
}
