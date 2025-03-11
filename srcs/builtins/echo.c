/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 17:34:36 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_shell *shell)
{
	t_arg	*current;
	int		n_flag;
	char	*expanded;

	current = shell->arguments->next;
	n_flag = 0;
	if (current && ft_strcmp(current->value, "-n") == 0)
	{
		n_flag = 1;
		current = current->next;
	}
	while (current)
	{
		expanded = expand_variables(shell, current->value);
		if (expanded)
		{
			printf("%s", expanded);
			free(expanded);
		}
		if (current->next)
			printf(" ");
		current = current->next;
	}
	if (!n_flag)
		printf("\n");
}
