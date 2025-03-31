/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 23:08:03 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execution(t_shell *shell)
{
	if (!shell || !shell->cmds)
	{
		shell->exit_status = 1;
		return ;
	}
	if (shell->cmds->pipe)
		pipeline(shell);
	else if (check_built_in(shell->cmds))
		execute_built_in(shell, shell->cmds);
	else
		execute_command(shell);
}

static void	process_input(t_shell *shell)
{
	shell->arguments = tokenize_input(shell->input, shell);
	if (shell->arguments)
	{
		shell->command = shell->arguments->value;
		shell->cmds = parse_tokens(shell, shell->arguments);
		if (shell->cmds)
		{
			execution(shell);
			free_commands(shell->cmds);
			shell->cmds = NULL;
		}
		free_arguments(shell->arguments);
		shell->arguments = NULL;
	}
	free(shell->input);
	shell->input = NULL;
}

static void	interactive_mode(t_shell *shell)
{
	while (1)
	{
		g_signal_num = 0;
		shell->input = readline("minishell$ ");
		if (!shell->input)
		{
			if (g_signal_num == SIGINT)
			{
				write(1, "\n", 1);
				g_signal_num = 0;
				continue ;
			}
			printf("exit\n");
			break ;
		}
		if (*shell->input)
		{
			add_history(shell->input);
			process_input(shell);
		}
		else
			free(shell->input);
	}
}

static void	non_interactive_mode(t_shell *shell)
{
	shell->input = readline("");
	if (shell->input && *shell->input)
		process_input(shell);
	else
		free(shell->input);
}

static int	command_mode(t_shell *shell, char *command)
{
	shell->input = ft_strdup(command);
	if (!shell->input)
		return (1);
	process_input(shell);
	return (shell->exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	int		interactive;
	int		status;

	interactive = isatty(STDIN_FILENO);
	init_shell(&shell, envp);
	setup_signals();
	if (argc > 2 && !ft_strcmp(argv[1], "-c"))
	{
		status = command_mode(&shell, argv[2]);
		free_shell(&shell);
		return (status);
	}
	if (interactive)
		interactive_mode(&shell);
	else
		non_interactive_mode(&shell);
	free_shell(&shell);
	return (shell.exit_status);
}
