/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/17 17:42:23 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void handle_input(t_shell *shell)
{
    shell->input = readline("minishell$ ");
    if (!shell->input)
    {
        printf("exit\n");
        clear_history();
        free_shell(shell);
        exit(0);
    }
    if (*shell->input != '\0')
        add_history(shell->input);
    shell->arguments = tokenize_input(shell->input, shell);
	//print_token_list(shell->arguments);
    if (shell->arguments)
        shell->command = shell->arguments->value;
    else
        shell->command = NULL;
}

void process_command(t_shell *shell)
{
    t_command *cmds_head = shell->cmds;
    shell->cmds = parse_tokens(shell->arguments);
    if (!shell->cmds)
    {
        free(shell->input);
        return;
    }
    if (shell->cmds->pipe)
        pipeline(shell);
    else
    {
        if (find_full_path(shell, shell->cmds->args[0]) == 0)
            execute_command(shell);
        else
        {
            write(2, "minishell: command not found: ", 30);
            write(2, shell->cmds->args[0], ft_strlen(shell->cmds->args[0]));
            write(2, "\n", 1);
        }
    }
    free_commands(cmds_head);
    free_arguments(shell->arguments);
    free(shell->input);
    shell->arguments = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(shell));
	init_shell(&shell, envp);
	//signal(SIGINT, handle_sigint);
	using_history();
	while (1)
	{
		handle_input(&shell);
		process_command(&shell);
	}
	clear_history();
	free_shell(&shell);
	return (0);
}
