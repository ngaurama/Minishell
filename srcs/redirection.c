/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/20 00:17:45 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int redirection(t_shell *shell)
{
    t_redir *redir;

    redir = shell->cmds->infiles;
    while (redir)
    {
        int fd;
        if (redir->type == T_REDIRECT_IN)
        {
            fd = open(redir->filename, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                return (1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (redir->type == T_HEREDOC)
        {
            fd = open(redir->filename, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                return (1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
    redir = shell->cmds->outfiles;
    while (redir)
    {
        int flags;
        if (redir->type == T_REDIRECT_OUT)
        {
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }
        else if (redir->type == T_APPEND)
        {
            flags = O_WRONLY | O_CREAT | O_APPEND;
        }
        else
        {
            write(2, "Invalid output redirection type\n", 32);
            return (1);
        }

        int fd = open(redir->filename, flags, 0644);
        if (fd == -1)
        {
            perror("open");
            return (1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
        redir = redir->next;
    }
    return (0);
}
