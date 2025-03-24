/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:08:46 by npbk              #+#    #+#             */
/*   Updated: 2025/03/24 01:38:04 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_isnumeric(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void		ft_exit(t_shell *shell)
{
	int		status;
	char	*arg;

	printf("exit\n");
	status = shell->exit_status;
	arg = shell->cmds->args[1];
	if (arg)
	{
		if (!ft_isnumeric(arg))
		{
			printf("exit: %s: numeric argument required\n", arg);
			free_shell(shell);
			exit(255);
		}
		if (shell->cmds->args[2])
		{
			printf("exit: too many arguments\n");
			shell->exit_status = 1;
			return;
		}
		status = ft_atoi(arg) % 256;
	}
	free_shell(shell);
	exit(status);
}
