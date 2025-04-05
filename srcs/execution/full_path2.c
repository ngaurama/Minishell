/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_path2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:54:43 by npagnon           #+#    #+#             */
/*   Updated: 2025/04/05 11:43:23 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*build_path(const char *dir, const char *command)
{
	char	*full_path;
	size_t	len;

	len = ft_strlen(dir) + ft_strlen(command) + 2;
	full_path = malloc(len);
	if (!full_path)
		return (NULL);
	full_path[0] = '\0';
	ft_strlcat(full_path, dir, len);
	ft_strlcat(full_path, "/", len);
	ft_strlcat(full_path, command, len);
	return (full_path);
}

char	*get_current_dir(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd && shell->current_dir)
		cwd = ft_strdup(shell->current_dir);
	if (!cwd)
		cwd = ft_strdup("/");
	return (cwd);
}

int	is_directory(const char *path, const char *command)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)command, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static void	print_permission_denied(t_shell *shell, const char *command)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)command, STDERR_FILENO);
	ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	shell->exit_status = 126;
	shell->err_printed = 1;
}

int	check_current_dir(t_shell *shell, const char *command, char *cwd)
{
	char	*full_path;

	full_path = build_path(cwd, command);
	if (!full_path)
		return (1);
	if (access(full_path, F_OK | X_OK) == 0)
	{
		if (is_directory(full_path, command))
		{
			free(full_path);
			return (1);
		}
		if (shell->full_path)
			free(shell->full_path);
		shell->full_path = full_path;
		return (0);
	}
	else if (access(full_path, F_OK) == 0)
	{
		print_permission_denied(shell, command);
		free(full_path);
		return (1);
	}
	free(full_path);
	return (1);
}
