/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:15:15 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_cd(t_shell *shell)
{
    char *dir = shell->cmds->args[1];
    char cwd[1024];

    if (!dir || ft_strcmp(dir, "~") == 0)
        dir = get_env_value(shell->env, "HOME");

    if (chdir(dir) == -1)
    {
        perror("cd");
        return;
    }
    if (getcwd(cwd, sizeof(cwd)))
        set_env_var(shell, "PWD", cwd);
    else
        perror("getcwd");
}
