/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_choose.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 20:33:31 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_built_in(t_command *cmd)
{
	int			i;
	const char	*command;
	const char	*builtins[8];

	i = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	command = cmd->args[0];
	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	while (builtins[i])
	{
		if (ft_strncmp(command, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	choose_builtin(t_shell *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		ft_echo(shell, cmd);
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		ft_cd(shell);
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		ft_pwd(shell);
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		ft_export(shell);
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		ft_unset(shell);
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		ft_env(shell);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		ft_exit(shell);
}
