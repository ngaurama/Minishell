/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:34:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 01:45:29 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_env(t_shell *shell)
{
	int		i;
	char	*equal;

	i = 0;
	while (shell->env[i])
	{
		equal = ft_strchr(shell->env[i], '=');
		if (equal)
			printf("%s\n", shell->env[i]);
		i++;
	}
	shell->exit_status = 0;
}
