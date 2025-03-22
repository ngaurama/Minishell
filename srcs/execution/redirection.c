/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/21 17:08:05 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int handle_input_redirection(t_redir *redir)
{
    int fd;

    while (redir)
    {
        if (redir->type == T_REDIRECT_IN || redir->type == T_HEREDOC)
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

int redirection(t_shell *shell)
{
    if (handle_input_redirection(shell->cmds->infiles) != 0)
        return (1);
    if (handle_output_redirection(shell->cmds->outfiles) != 0)
        return (1);
    return (0);
}
