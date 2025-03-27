/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:05 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/27 14:32:47 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_signal_num = 0;

void handle_signal(int signum)
{
    g_signal_num = signum;

    if (signum == SIGINT)
    {
        rl_on_new_line();
        printf("\n");
        //rl_replace_line("", 0);
        rl_redisplay();
    }
}

void setup_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
    sa.sa_handler = handle_signal;
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}

void handle_input(t_shell *shell)
{
    shell->input = readline("minishell$ ");
    if (!shell->input)
    {
        printf("exit\n");
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

void execution(t_shell *shell)
{
    if (shell->cmds->pipe)
        pipeline(shell);
    else
    {
        if (check_built_in(shell->cmds))
            execute_built_in(shell, shell->cmds);
        else
        {
            if (execute_command(shell) == 0)
            {
                int status;
                pid_t waited_pid = waitpid(shell->pid, &status, 0);
                if (waited_pid == -1)
                    perror("waitpid failed");
                else
                    update_exit_status(shell, status);
            }
        }
    }
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell;

    (void)argc;
    (void)argv;
    init_shell(&shell, envp);
    setup_signals();
    while (1)
    {
        handle_input(&shell);
        if (shell.arguments)
        {
            shell.cmds = parse_tokens(shell.arguments);
            //print_command_list(shell.cmds);
            if (shell.cmds)
            {
                execution(&shell);
                free_commands(shell.cmds);
				shell.cmds = NULL;
            }
            free_arguments(shell.arguments);
			shell.arguments = NULL;
        }
    }
    clear_history();
    free_shell(&shell);
    return (0);
}
