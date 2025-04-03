/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 12:59:42 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	save_and_redirect_fds(int *saved_stdin, int *saved_stdout,
			t_command *cmd, t_shell *shell)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("dup failed");
		return (1);
	}
	if (redirection(cmd, shell) != 0)
	{
		close(*saved_stdin);
		close(*saved_stdout);
		return (1);
	}
	return (0);
}

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

int	setup_builtin_redirections(t_shell *shell, t_command *cmd,
	int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = -1;
	*saved_stdout = -1;
	if (!cmd->pipe)
	{
		if (save_and_redirect_fds(saved_stdin, saved_stdout, cmd, shell) != 0)
		{
			shell->exit_status = 1;
			shell->redir_err = 0;
			return (1);
		}
	}
	if (shell->redir_err)
	{
		shell->exit_status = 1;
		if (*saved_stdin != -1 && *saved_stdout != -1)
			restore_fds(*saved_stdin, *saved_stdout);
		shell->redir_err = 0;
		return (1);
	}
	return (0);
}

void	execute_built_in(t_shell *shell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;

	if (setup_builtin_redirections(shell, cmd, &saved_stdin, &saved_stdout))
		return ;
	if (!cmd->args[0])
	{
		shell->exit_status = 1;
		if (saved_stdin != -1 && saved_stdout != -1)
			restore_fds(saved_stdin, saved_stdout);
		return ;
	}
	choose_builtin(shell, cmd);
	if (saved_stdin != -1 && saved_stdout != -1)
		restore_fds(saved_stdin, saved_stdout);
}
