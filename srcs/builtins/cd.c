/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 22:13:01 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*recover_pwd(char *oldpwd, char *dir, t_shell *shell)
{
	char	*new_dir;
	char	*tmp;

	new_dir = NULL;
	if (ft_strncmp(dir, "..", 3) == 0)
		new_dir = ft_strjoin(oldpwd, "/..");
	else if (dir[0] == '/')
		new_dir = ft_strdup(dir);
	else
	{
		tmp = ft_strjoin(oldpwd, "/");
		new_dir = ft_strjoin(tmp, dir);
		free(tmp);
	}
	ft_putstr_fd(
		"cd: error retrieving current directory: getcwd: "
		"cannot access parent directories: No such file or directory\n",
		STDERR_FILENO);
	shell->exit_status = 1;
	return (new_dir);
}

static int	change_directory(char *dir, char *oldpwd, t_shell *shell)
{
	char	*new_dir;

	if (chdir(dir) == -1)
		return (print_cd_error(dir, shell), 1);
	new_dir = getcwd(NULL, 0);
	if (!new_dir)
		new_dir = recover_pwd(oldpwd, dir, shell);
	set_env_var(shell, "OLDPWD", oldpwd);
	set_env_var(shell, "PWD", new_dir);
	free(shell->current_dir);
	shell->current_dir = new_dir;
	return (0);
}

static char	*get_oldpwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (shell->current_dir)
			return (ft_strdup(shell->current_dir));
		return (ft_strdup("/"));
	}
	return (cwd);
}

static char	*get_cd_target(t_shell *shell, int *free_dir)
{
	char	*dir;

	dir = shell->cmds->args[1];
	if (!dir)
	{
		dir = get_env_value(shell->env, "HOME");
		if (!dir)
		{
			print_error("cd: HOME not set", NULL, NULL);
			return (NULL);
		}
		*free_dir = 1;
	}
	else if (ft_strncmp(dir, "-", 2) == 0)
	{
		dir = get_env_value(shell->env, "OLDPWD");
		if (!dir)
		{
			print_error("cd: OLDPWD not set", NULL, NULL);
			return (NULL);
		}
		*free_dir = 1;
		printf("%s\n", dir);
	}
	return (dir);
}

void	ft_cd(t_shell *shell)
{
	char	*dir;
	char	*oldpwd;
	int		free_dir;

	if (!validate_cd_args(shell))
		return ;
	oldpwd = get_oldpwd(shell);
	if (!oldpwd)
		return ;
	free_dir = 0;
	dir = get_cd_target(shell, &free_dir);
	if (!dir)
	{
		free(oldpwd);
		shell->exit_status = 1;
		return ;
	}
	change_directory(dir, oldpwd, shell);
	free(oldpwd);
	if (free_dir)
		free(dir);
}
