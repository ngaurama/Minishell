/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:32:10 by ngaurama         ###   ########.fr       */
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

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    ft_memset(&shell, 0, sizeof(shell));
    init_shell(&shell, envp);
    //using_history();
    while (1)
    {
        handle_input(&shell);
        if (shell.arguments)
        {
            shell.cmds = parse_tokens(shell.arguments);
            if (shell.cmds)
            {
                if (shell.cmds->pipe)
                    pipeline(&shell);
                else
                    execute_command(&shell);
                free_commands(shell.cmds);
            }
            free_arguments(shell.arguments);
        }
        free(shell.input);
    }
    clear_history();
    free_shell(&shell);
    return (0);
}