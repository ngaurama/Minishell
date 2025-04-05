/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 14:18:17 by npbk              #+#    #+#             */
/*   Updated: 2025/04/05 10:58:45 by npagnon          ###   ########.fr       */
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

static int	is_delimiter(char *line, char *expanded,
		const char *delimiter, int expand)
{
	if ((!expand && ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		|| (expand && ft_strncmp(expanded, delimiter, ft_strlen(delimiter)
				+ 1) == 0))
	{
		free(line);
		if (expand)
			free(expanded);
		return (1);
	}
	return (0);
}

int	stop_heredoc(char *line, const char *delimiter, t_arg *tok, t_shell *shell)
{
	char	*expanded;
	int		expand;

	expanded = NULL;
	expand = (tok->should_expand || !tok->quoted);
	if (!line)
	{
		ft_putstr_fd("minishell: warning: ", 2);
		ft_putstr_fd("here-document delimited by end-of-file\n", 2);
		return (1);
	}
	if (expand)
		expanded = heredoc_expand(line, shell);
	if (is_delimiter(line, expanded, delimiter, expand))
		return (1);
	if (expand)
		free(expanded);
	return (0);
}

void	write_heredoc_line(int fd, char *line, t_shell *shell, t_arg *tok)
{
	char	*expanded;

	if (!tok->quoted)
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
