/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:17:34 by npbk              #+#    #+#             */
/*   Updated: 2025/04/03 16:38:59 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	child_error_and_exit(t_shell *shell, const char *cmd)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("minishell: ", cmd);
	if (!tmp)
		free_and_exit(shell, 127);
	msg = ft_strjoin(tmp, ": command not found\n");
	free(tmp);
	if (!msg)
		free_and_exit(shell, 127);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
	free_and_exit(shell, 127);
}
