/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/05 14:34:10 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		setup_signals(shell);
		shell->input = readline("minishell$ ");
		if (!shell->input)
		{
			if (g_signal_num == SIGINT)
			{
				write(1, "\n", 1);
				g_signal_num = 0;
				setup_signals(shell);
				continue ;
			}
			ft_putstr_fd("exit\n", STDERR_FILENO);
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
	char	*line;
	size_t	len;

	line = NULL;
	line = get_next_line(STDIN_FILENO);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		shell->input = ft_strdup(line);
		free(line);
		if (shell->input && *(shell->input))
			process_input(shell);
		else if (shell->input)
			free(shell->input);
		line = get_next_line(STDIN_FILENO);
	}
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
	setup_signals(&shell);
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
