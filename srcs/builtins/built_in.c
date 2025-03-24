/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/24 01:37:49 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		save_and_redirect_fds(int *saved_stdin, int *saved_stdout, t_shell *shell)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdin == -1 || *saved_stdout == -1)
    {
        perror("dup failed");
        return (1);
    }
    if (redirection(shell) != 0)
    {
        close(*saved_stdin);
        close(*saved_stdout);
        return (1);
    }
    return (0);
}

int		check_built_in(t_command *cmd)
{
	int			i;
	const char	*command;
	const char	*builtins[8];

	i = 0;
	if (!cmd || !cmd->args[0])
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

void	execute_built_in(t_shell *shell)
{
	int 	saved_stdin;
	int 	saved_stdout;
	char 	*cmd;

	if (save_and_redirect_fds(&saved_stdin, &saved_stdout, shell) != 0)
		return;
	cmd = shell->cmds->args[0];
	if (ft_strncmp(cmd, "echo", 5) == 0)
		ft_echo(shell);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		ft_cd(shell);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		ft_pwd(shell);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		ft_export(shell);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		ft_unset(shell);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		ft_env(shell);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		ft_exit(shell);
	restore_fds(saved_stdin, saved_stdout);
}
