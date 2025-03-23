/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/23 17:57:19 by npbk             ###   ########.fr       */
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
			return (perror("cd: HOME not set"), NULL);
	}
	else if (ft_strncmp(dir, "-", 2) == 0)
	{
		dir = get_env_value(shell->env, "OLDPWD");
		if (!dir)
			return (perror("cd: OLDPWD not set"), NULL);
		printf("%s\n", dir);
	}
	return (dir);
}

void	    ft_cd(t_shell *shell)
{
	char	*dir;
	char	cwd[1024];
	char	oldpwd[1024];

    dir = shell->cmds->args[1];
	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
		perror("getcwd");
	dir = get_target_dir(shell);
    if (!dir)
        return ;
    if (chdir(dir) == -1)
	    return (perror("cd"));
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_var(shell, "PWD", cwd);
		set_env_var(shell, "OLDPWD", oldpwd);
	}
	else
		perror("getcwd");
	if (dir != shell->cmds->args[1])
		free(dir);
}
