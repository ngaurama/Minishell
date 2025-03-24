/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/24 01:38:00 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	parse_n_flag(char **args, int *start_index)
{
	int	i;
	int	j;
	int	newline;

	i = *start_index;
	newline = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break;
		newline = 0;
		i++;
	}
	*start_index = i;
	return (newline);
}

void		ft_echo(t_shell *shell)
{
	int	i;
	int	newline;

	i = 1;
	newline = parse_n_flag(shell->cmds->args, &i);
	while (shell->cmds->args[i])
	{
		printf("%s", shell->cmds->args[i]);
		if (shell->cmds->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	shell->exit_status = 0;
}
