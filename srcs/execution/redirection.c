/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/26 03:05:21 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_heredoc(const char *delimiter)
{
    int pipefd[2];
    char *line;
    
    if (pipe(pipefd) == -1)
        return (perror("pipe"), -1);
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_putstr_fd("minishell: warning: here-document delimited by end-of-file\n", 2);
            break;
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipefd[1], line, ft_strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    return pipefd[0];
}

int handle_input_redirection(t_redir *redir)
{
    int fd;

    while (redir)
    {
        if (redir->type == T_REDIRECT_IN)
        {
            if (ft_isdigit(redir->filename[0]))
                fd = ft_atoi(redir->filename);
            else
                fd = open(redir->filename, O_RDONLY);
            if (fd == -1)
                return (perror(redir->filename), 1);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == T_HEREDOC)
        {
            fd = handle_heredoc(redir->filename);
            if (fd == -1)
                return (1);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
    return (0);
}

int handle_output_redirection(t_redir *redir)
{
    int fd;
    int flags;

    while (redir)
    {
        if (redir->type == T_REDIRECT_OUT)
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        else if (redir->type == T_APPEND)
            flags = O_WRONLY | O_CREAT | O_APPEND;
        else
        {
            write(2, "Invalid output redirection type\n", 32);
            return (1);
        }
        fd = open(redir->filename, flags, 0644);
        if (fd == -1)
            return (perror("open"), 1);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        redir = redir->next;
    }
    return (0);
}

int redirection(t_command *cmd)
{
    if (handle_input_redirection(cmd->heredocs) != 0)
        return (1);
    if (handle_input_redirection(cmd->infiles) != 0)
        return (1);
    if (handle_output_redirection(cmd->outfiles) != 0)
        return (1);
    return (0);
}