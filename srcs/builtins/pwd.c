/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:40 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 23:03:16 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_shell *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		printf("%s\n", cwd);
		free(cwd);
		shell->exit_status = 0;
	}
	else if (shell->current_dir)
	{
		printf("%s\n", shell->current_dir);
		shell->exit_status = 0;
	}
	else
	{
		perror("pwd failed");
		shell->exit_status = 1;
	}
}
