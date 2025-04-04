/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:54:15 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 19:54:47 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile sig_atomic_t	g_signal_num = 0;

void	handle_signal(int signum)
{
	g_signal_num = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		if (rl_done == 0)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			g_signal_num = 0;
		}
	}
}

static int	set_signal_handler(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(sig, &sa, NULL) == -1)
		return (-1);
	return (0);
}

void	setup_signals(t_shell *shell)
{
	if (set_signal_handler(SIGINT, handle_signal) < 0
		|| set_signal_handler(SIGQUIT, SIG_IGN) < 0
		|| set_signal_handler(SIGTSTP, SIG_IGN) < 0
		|| set_signal_handler(SIGTERM, handle_signal) < 0)
	{
		perror("sigaction");
		free_shell(shell);
		exit(1);
	}
}

void	handle_signal_heredoc(int signum)
{
	g_signal_num = signum;
	if (signum == SIGINT)
	{
		//rl_replace_line("", 0);
		//rl_on_new_line();
		//rl_redisplay();
		//g_signal_num = SIGINT;
		rl_done = 1;
	}
}

int	rl_hook()
{
	if (g_signal_num)
	{
		//rl_replace_line("", 0);
		rl_done = 1;
	}
	return 0;
}
