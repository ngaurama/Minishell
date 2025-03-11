/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 14:24:44 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void handle_sigint(int sig)
// {
//     (void)sig;
//     write(1, "\n", 1);
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
// }

void print_path(void)
{
    char *path = getenv("PATH");
    if (path)
        printf("PATH: %s\n", path);
    else
        printf("PATH not set\n");
}

void handle_input(t_shell *shell)
{
    char *token;

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
    token = ft_strtok(shell->input, " ");
    shell->arguments = NULL;
    while (token)
    {
        shell->arguments = add_argument(shell->arguments, token);
        token = ft_strtok(NULL, " ");
    }
    if (shell->arguments)
        shell->command = shell->arguments->value;
    else
        shell->command = NULL;
}

void process_command(t_shell *shell)
{
    if (!shell->command)
    {
        free(shell->input);
        return;
    }
    if (check_built_in(shell) == 1)
        execute_built_in(shell);
    else
    {
        if (find_full_path(shell) == 0)
            execute_command(shell);
        else
        {
            write(2, "minishell: command not found: ", 30);
            write(2, shell->command, ft_strlen(shell->command));
            write(2, "\n", 1);
        }
    }
    //print_path();
    free_arguments(shell->arguments);
    free(shell->input);
    shell->arguments = NULL;
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    
    (void)argc;
    (void)argv;
    memset(&shell, 0, sizeof(shell));
    init_shell(&shell, envp);
    // signal(SIGINT, handle_sigint);
    using_history();
    while (1)
    {
        handle_input(&shell);    
        process_command(&shell);
    }
    clear_history();
    free_shell(&shell);
    return 0;
}
