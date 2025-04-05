/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:50:00 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/05 11:21:08 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	search_path(t_shell *shell, const char *command, char *path_var_copy)
{
	char	*path;

	path = ft_strtok(path_var_copy, ":");
	while (path)
	{
		shell->full_path = build_path(path, command);
		if (!shell->full_path)
			return (free(path_var_copy), 1);
		if (access(shell->full_path, F_OK | X_OK) == 0)
		{
			if (is_directory(shell->full_path, command))
			{
				free(shell->full_path);
				shell->full_path = NULL;
				free(path_var_copy);
				return (1);
			}
			return (free(path_var_copy), 0);
		}
		free(shell->full_path);
		shell->full_path = NULL;
		path = ft_strtok(NULL, ":");
	}
	free(path_var_copy);
	return (1);
}

int	search_path_var(t_shell *shell, const char *command)
{
	char	*path_var;
	char	*path_var_copy;
	int		result;

	path_var = get_env_value(shell->env, "PATH");
	if (!path_var || !*path_var)
	{
		free(path_var);
		return (1);
	}
	path_var_copy = ft_strdup(path_var);
	free(path_var);
	if (!path_var_copy)
		return (1);
	if (shell->full_path)
		free(shell->full_path);
	shell->full_path = NULL;
	result = search_path(shell, command, path_var_copy);
	return (result);
}

int	direct_path(t_shell *shell, const char *command)
{
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd((char *)command, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			shell->exit_status = 126;
			shell->err_printed = 1;
			return (1);
		}
		if (is_directory(command, command))
		{
			shell->exit_status = 126;
			shell->err_printed = 1;
			return (2);
		}
		if (shell->full_path)
			free(shell->full_path);
		shell->full_path = ft_strdup(command);
		if (!shell->full_path)
			return (1);
		return (0);
	}
	return (-1);
}

static int	handle_empty_path_var(t_shell *shell, const char *command)
{
	char	*cwd;
	int		result;

	cwd = get_current_dir(shell);
	if (!cwd)
		return (1);
	result = check_current_dir(shell, command, cwd);
	free(cwd);
	if (result == 0)
		return (0);
	else
		return (1);
}

int	find_full_path(t_shell *shell, const char *command)
{
	int		direct_result;
	char	*path_var;

	if (!command)
		return (1);
	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
	{
		direct_result = direct_path(shell, command);
		return (direct_result);
	}
	direct_result = search_path_var(shell, command);
	if (direct_result == 0)
		return (0);
	path_var = get_env_value(shell->env, "PATH");
	if (!path_var || !*path_var)
	{
		free(path_var);
		return (handle_empty_path_var(shell, command));
	}
	free(path_var);
	return (1);
}
