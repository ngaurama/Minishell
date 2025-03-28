/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/28 19:22:37 by npagnon          ###   ########.fr       */
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
	char	cwd[1024];

	if (chdir(dir) == -1)
	{
		perror("cd");
		shell->exit_status = 1;
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_var(shell, "PWD", cwd);
		set_env_var(shell, "OLDPWD", oldpwd);
	}
	else
		perror("getcwd");
	shell->exit_status = 0;
	return (0);
}

void	ft_cd(t_shell *shell)
{
	char	*dir;
	char	oldpwd[1024];

	dir = shell->cmds->args[1];
	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		perror("getcwd");
	dir = get_target_dir(shell);
	if (!dir)
	{
		shell->exit_status = 1;
		return ;
	}
	if (change_directory(dir, oldpwd, shell) && dir != shell->cmds->args[1])
		free(dir);
	else if (dir != shell->cmds->args[1])
		free(dir);
}
