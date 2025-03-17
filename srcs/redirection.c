/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/16 21:31:53 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void redirection(t_shell *shell)
{
    t_arg *current = shell->arguments;
    t_arg *prev = NULL;

    while (current)
    {
        if (current->type == T_REDIRECT_IN || current->type == T_REDIRECT_OUT || current->type == T_APPEND)
        {
            if (!current->next || current->next->type != T_WORD)
            {
                if (current->type == T_REDIRECT_IN)
                    write(2, "Syntax error: missing filename after `<`\n", 41);
                else if (current->type == T_REDIRECT_OUT)
                    write(2, "Syntax error: missing filename after `>`\n", 41);
                else if (current->type == T_APPEND)
                    write(2, "Syntax error: missing filename after `>>`\n", 42);
                exit(1);
            }
            int flags;
            if (current->type == T_REDIRECT_OUT)
                flags = O_WRONLY | O_CREAT | O_TRUNC;
            else if (current->type == T_APPEND)
                flags = O_WRONLY | O_CREAT | O_APPEND;
            else
                flags = O_RDONLY;
            int fd = open(current->next->value, flags, 0644);
            if (fd == -1)
            {
                perror("open");
                exit(1);
            }
            int target_fd;
            if (current->type == T_REDIRECT_IN)
                target_fd = STDIN_FILENO;
            else
                target_fd = STDOUT_FILENO;
            if (dup2(fd, target_fd) == -1)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }
            close(fd);
            t_arg *next_next = current->next->next;
            free(current->next);
            free(current);

            if (prev)
                prev->next = next_next;
            else
                shell->arguments = next_next;

            current = next_next;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}
