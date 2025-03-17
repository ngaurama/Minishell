/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/17 20:42:16 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_echo(t_shell *shell)
{
	t_arg	*current;
	int		n_flag;

	current = shell->arguments->next;
	n_flag = 0;
	if (current && ft_strncmp(current->value, "-n", 3) == 0)
	{
		n_flag = 1;
		current = current->next;
	}
	while (current)
	{
		printf("%s", current->value);
		if (current->next)
			printf(" ");
		current = current->next;
	}
	if (!n_flag)
		printf("\n");
}
