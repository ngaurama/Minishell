/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:55:24 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 13:23:28 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_heredoc(const char *delimiter, t_shell *shell, int expand)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (stop_heredoc(line, delimiter))
			break;
		write_heredoc_line(pipefd[1], line, shell, expand);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int handle_input_redirection(t_redir *redir, t_shell *shell)
{
    int fd;

    while (redir)
    {
        if (redir->type == T_REDIRECT_IN)
            fd = handle_redirect_in_file(redir->filename);
        else if (redir->type == T_HEREDOC)
            fd = handle_heredoc_input(redir, shell);
        else
            return (1);
        if (fd == -1)
            return (1);

        dup2(fd, STDIN_FILENO);
        close(fd);
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
            ft_putstr_fd("Invalid output redirection type\n", STDERR_FILENO);
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

int redirection(t_command *cmd, t_shell *shell)
{
    if (handle_input_redirection(cmd->heredocs, shell) != 0)
        return (1);
    if (handle_input_redirection(cmd->infiles, shell) != 0)
        return (1);
    if (handle_output_redirection(cmd->outfiles) != 0)
        return (1);
    return (0);
}
