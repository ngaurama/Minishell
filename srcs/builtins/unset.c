/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:53 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 17:35:03 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_unset(t_shell *shell)
{
	t_arg *current = shell->arguments->next;

	while (current)
	{
		char *key = current->value;
		int i = 0;
		while (shell->env[i])
		{
			if (ft_strncmp(shell->env[i], key, ft_strlen(key)) == 0
				&& shell->env[i][ft_strlen(key)] == '=')
			{
				free(shell->env[i]);
				int j = i;
				while (shell->env[j + 1])
				{
					shell->env[j] = shell->env[j + 1];
					j++;
				}
				shell->env[j] = NULL;
				break ;
			}
			i++;
		}
		current = current->next;
	}
}