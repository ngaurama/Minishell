/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:54:15 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/31 16:51:36 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//check cat and ctrl+c when rl_replace_line is being used

volatile sig_atomic_t	g_signal_num = 0;

void	handle_signal(int signum)
{
	g_signal_num = signum;

	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		if (rl_done == 0) // GNU Readline internal flag that tells if the line is still active
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			g_signal_num = 0;
		}
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sa.sa_flags = 0; //adjusted to fix cat and ctrl+c
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
    if (sigaction(SIGTSTP, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
	sa.sa_handler = handle_signal;
	if (sigaction(SIGTERM, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}
