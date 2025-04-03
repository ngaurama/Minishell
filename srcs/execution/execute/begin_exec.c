/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:33:06 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 16:42:25 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	execution(t_shell *shell)
{
	if (!shell || !shell->cmds)
	{
		shell->exit_status = 1;
		return ;
	}
	if (shell->cmds->pipe)
		pipeline(shell);
	else if (check_built_in(shell->cmds))
		execute_built_in(shell, shell->cmds);
	else
		execute_command(shell);
}
