/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:31:34 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void pipeline(t_shell *shell)
{
    t_command *cmd = shell->cmds;
    int pipefd[2];
    int prev_pipe_read = -1;
    pid_t pid;

    while (cmd)
    {
        if (cmd->pipe && pipe(pipefd) == -1)
        {
            perror("pipe");
            return;
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
                close(pipefd[0]);
                close(pipefd[1]);
            }
            else if (cmd->outfiles)
            {
                t_redir *outfile = cmd->outfiles;
                int flags = O_WRONLY | O_CREAT;
                if (outfile->type == T_APPEND)
                    flags |= O_APPEND;
                else
                    flags |= O_TRUNC;
                int fd = open(outfile->filename, flags, 0644);
                if (fd == -1)
                {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            redirection(shell);
            if (check_built_in(cmd))
            {
                execute_built_in(shell);
                exit(0);
            }
            else
            {
                if (find_full_path(shell, cmd->args[0]) != 0)
                {
                    write(2, "minishell: command not found: ", 30);
                    write(2, cmd->args[0], ft_strlen(cmd->args[0]));
                    write(2, "\n", 1);
                    exit(1);
                }
                execve(shell->full_path, cmd->args, shell->env);
                perror("execve failed");
                exit(1);
            }
        }
        else if (pid > 0)
        {
            if (prev_pipe_read != -1)
            {
                close(prev_pipe_read);
            }
            if (cmd->pipe)
            {
                close(pipefd[1]);
                prev_pipe_read = pipefd[0];
            }
        }
        else
        {
            perror("fork failed");
            return;
        }

        cmd = cmd->next;
    }
    if (prev_pipe_read != -1)
    {
        close(prev_pipe_read);
    }
    while (wait(NULL) > 0)
        ;
}