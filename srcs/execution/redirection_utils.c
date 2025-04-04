/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:18:17 by npbk              #+#    #+#             */
/*   Updated: 2025/04/02 12:42:16 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redirect_in_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	handle_heredoc_input(t_redir *redir, t_shell *shell)
{
	int	expand;

	expand = 1;
	if (redir->src_token)
		expand = (redir->src_token->quoted == 0);
	return (handle_heredoc(redir->filename, shell, expand));
}

int	stop_heredoc(char *line, const char *delimiter)
{
	if (!line)
	{
		ft_putstr_fd("minishell: warning: ", 2);
		ft_putstr_fd("here-document delimited by end-of-file\n", 2);
		return (1);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

void	write_heredoc_line(int fd, char *line, t_shell *shell, int expand)
{
	char	*expanded;

	if (expand)
	{
		expanded = heredoc_expand(line, shell);
		write(fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	handle_inredir_error(t_redir *redir, t_shell *shell)
{
	if (redir->filename)
		perror(redir->filename);
	else
		perror("heredoc");
	shell->redir_err = 1;
	return (1);
}
