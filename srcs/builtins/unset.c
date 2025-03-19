/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:53 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:34:30 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_unset(t_shell *shell)
{
    int i = 1;

    while (shell->cmds->args[i])
    {
        unset_env_var(shell, shell->cmds->args[i]);
        i++;
    }
}
