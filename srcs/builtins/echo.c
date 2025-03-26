/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:10 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/26 03:01:43 by ngaurama         ###   ########.fr       */
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

void ft_echo(t_shell *shell, t_command *cmd)
{
    int i = 1;
    int newline;
    int len;
	
    newline = parse_n_flag(cmd->args, &i);
    while (cmd->args[i])
    {
        len = ft_strlen(cmd->args[i]);
        write(STDOUT_FILENO, cmd->args[i], len);
        if (cmd->args[i + 1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (newline)
        write(STDOUT_FILENO, "\n", 1);
    shell->exit_status = 0;    
}