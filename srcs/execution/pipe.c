/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:58:30 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/26 03:00:54 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void pipeline_init(t_shell *shell, int buffer[2], int *prev_pipe_read)
{
    preprocess_heredocs(shell->cmds);
    if (pipe(buffer) == -1) 
    {
        perror("pipe");
        return;
    }
    *prev_pipe_read = -1;
}

void pipeline_handle_special(t_shell *shell, t_command **cmd, int pipefd[2])
{
    pid_t writer_pid = fork();
    if (writer_pid == 0) 
    {
        setup_child_pipes(-1, pipefd, *cmd);
        execute_child_pipes(shell, *cmd);
    }
    waitpid(writer_pid, NULL, 0);
    *cmd = (*cmd)->next;
    pid_t reader_pid = fork();
    if (reader_pid == 0) 
    {
        setup_child_pipes(-1, pipefd, *cmd);
        execute_child_pipes(shell, *cmd);
    }
    waitpid(reader_pid, NULL, 0);
    *cmd = (*cmd)->next;
}

void pipeline_handle_regular(t_shell *shell, t_command *cmd, int buffer[2], int *prev_pipe_read, int pipefd[2])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        close(buffer[0]);
        dup2(buffer[1], STDERR_FILENO);
        close(buffer[1]);
        setup_child_pipes(*prev_pipe_read, pipefd, cmd);
        execute_child_pipes(shell, cmd);
    }
    else if (pid > 0)
    {
        if (*prev_pipe_read != -1)
            close(*prev_pipe_read);
        if (cmd->pipe)
        {
            close(pipefd[1]);
            *prev_pipe_read = pipefd[0];
        }
    }
    else
    {
        perror("fork failed");
        close(buffer[0]);
        close(buffer[1]);
    }
}

void pipeline_process(t_shell *shell, int buffer[2], int *prev_pipe_read)
{
    t_command *cmd = shell->cmds;
    int pipefd[2];

    while (cmd)
    {
        if (cmd->outfiles && cmd->pipe && cmd->next)
        {
            pipeline_handle_special(shell, &cmd, pipefd);
            continue;
        }
        if (cmd->pipe && pipe(pipefd) == -1)
        {
            perror("pipe");
            close(buffer[0]);
            close(buffer[1]);
            return;
        }
        pipeline_handle_regular(shell, cmd, buffer, prev_pipe_read, pipefd);
        cmd = cmd->next;
    }
}

void pipeline_cleanup(int buffer[2], int prev_pipe_read)
{
    char buf[1024];
    ssize_t n;

    close(buffer[1]);
    n = read(buffer[0], buf, sizeof(buf)-1);
    while (n > 0)
    {
        buf[n] = '\0';
        write(STDERR_FILENO, buf, n);
        n = read(buffer[0], buf, sizeof(buf)-1);
    }
    close(buffer[0]);
    if (prev_pipe_read != -1)
        close(prev_pipe_read);
    while (wait(NULL) > 0)
        ;
}

void pipeline(t_shell *shell)
{
    int buffer[2];
    int prev_pipe_read;

    pipeline_init(shell, buffer, &prev_pipe_read);
    pipeline_process(shell, buffer, &prev_pipe_read);
    pipeline_cleanup(buffer, prev_pipe_read);
}
