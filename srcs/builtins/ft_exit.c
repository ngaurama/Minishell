/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:08:46 by npbk              #+#    #+#             */
/*   Updated: 2025/04/05 14:34:06 by npagnon          ###   ########.fr       */
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

void	not_numeric_arg_exit(t_shell *shell, const char *arg)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("exit: ", arg);
	if (!tmp)
	{
		free_shell(shell);
		exit(2);
	}
	msg = ft_strjoin(tmp, ": numeric argument required\n");
	free(tmp);
	if (!msg)
	{
		free_shell(shell);
		exit(2);
	}
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
	free_shell(shell);
	exit(2);
}

int	handle_exit_errors(t_shell *shell, char *arg)
{
	if (arg && !ft_isnumeric(arg))
		not_numeric_arg_exit(shell, arg);
	if (arg && shell->cmds->args[2])
	{
		write(STDERR_FILENO, "exit: too many arguments\n", 26);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

void	ft_exit(t_shell *shell)
{
	long long		status;
	char			*arg;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	status = shell->exit_status;
	arg = shell->cmds->args[1];
	if (handle_exit_errors(shell, arg))
		return ;
	if (arg)
		status = ft_atoll(arg) % 256;
	free_shell(shell);
	exit(status);
}
