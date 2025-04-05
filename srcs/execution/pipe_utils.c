/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:53:58 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/05 11:45:20 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_fork_error(int buffer[2])
{
	perror("fork failed");
	close(buffer[0]);
	close(buffer[1]);
}

void	free_and_exit(t_shell *shell, int exit_code)
{
	int	fd;

	if (shell)
		free_shell(shell);
	fd = 2;
	while (++fd < 1024)
		close(fd);
	exit(exit_code);
}

void	pipeline_init(t_shell *shell, int buffer[2], int *prev_pipe_read)
{
	preprocess_heredocs(shell->cmds, shell);
	if (pipe(buffer) == -1)
	{
		perror("pipe");
		return ;
	}
	*prev_pipe_read = -1;
}

void	print_signal_msg(int sig)
{
	if (sig == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
	else if (sig == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
}

void	setup_stderr_pipe(int buffer[2])
{
	close(buffer[0]);
	if (dup2(buffer[1], STDERR_FILENO) == -1)
	{
		perror("dup2 stderr");
		exit(1);
	}
	close(buffer[1]);
}
