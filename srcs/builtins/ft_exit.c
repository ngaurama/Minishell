/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:08:46 by npbk              #+#    #+#             */
/*   Updated: 2025/03/27 16:43:16 by npbk             ###   ########.fr       */
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

static int	handle_exit_errors(t_shell *shell, char *arg)
{
	if (arg && !ft_isnumeric(arg))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_shell(shell);
		exit(2);
	}
	if (arg && shell->cmds->args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

void		ft_exit(t_shell *shell)
{
	int		status;
	char	*arg;

	ft_putstr_fd("exit\n", 1);
	status = shell->exit_status;
	arg = shell->cmds->args[1];
	if (handle_exit_errors(shell, arg))
		return;
	if (arg)
		status = ft_atoi(arg) % 256;
	free_shell(shell);
	exit(status);
}

