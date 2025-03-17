/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/17 17:37:29 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pipeline(t_shell *shell)
{
    t_command *cmd = shell->cmds;
    int pipefd[2];
    int prev_pipe_read = -1;
    pid_t pid;
    int status;

    while (cmd)
    {
        if (check_built_in(shell))
        {
            execute_built_in(shell);
            cmd = cmd->next;
            continue;
        }
        if (find_full_path(shell, cmd->args[0]))
        {
            write(2, "minishell: command not found: ", 30);
            write(2, cmd->args[0], ft_strlen(cmd->args[0]));
            write(2, "\n", 1);
            return;
        }
        if (cmd->pipe)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                if (prev_pipe_read != -1)
                    close(prev_pipe_read);
                return;
            }
        }
        pid = fork();
        if (pid == 0)
        {
            if (prev_pipe_read != -1)
            {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            if (cmd->pipe)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }
            if (prev_pipe_read != -1)
                close(prev_pipe_read);
            if (cmd->pipe)
                close(pipefd[0]);
            redirection(shell);
            if (execve(shell->full_path, cmd->args, shell->env) == -1)
            {
                perror("execve failed");
                exit(1);
            }
        }
        else if (pid > 0)
        {
            if (cmd->pipe)
                close(pipefd[1]);
            if (prev_pipe_read != -1)
                close(prev_pipe_read);
            if (cmd->pipe)
                prev_pipe_read = pipefd[0];
            else
                prev_pipe_read = -1;
            cmd = cmd->next;
        }
        else
        {
            perror("fork failed");
            if (prev_pipe_read != -1)
                close(prev_pipe_read);
            if (cmd->pipe)
            {
                close(pipefd[0]);
                close(pipefd[1]);
            }
            return;
        }
    }
    if (prev_pipe_read != -1)
        close(prev_pipe_read);
    while (wait(&status) > 0)
        ;
}
