/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:53:58 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/30 00:01:57 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_and_exit(t_shell *shell, int exit_code)
{
	if (shell)
		free_shell(shell);
	exit(exit_code);
}

void 	setup_child_pipes(int prev_pipe_read, int pipefd[2], t_command *cmd)
{
    if (prev_pipe_read != -1 && !cmd->infiles && !cmd->heredocs)
    {
        dup2(prev_pipe_read, STDIN_FILENO);
        close(prev_pipe_read);
    }
    if (cmd->pipe && !cmd->outfiles)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
    }
}

void 	execute_child_pipes(t_shell *shell, t_command *cmd)
{
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0') 
	{
        ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
        free_and_exit(shell, 127);
    }
    if (redirection(cmd, shell) != 0)
		free_and_exit(shell, 1);
    if (check_built_in(cmd))
    {
        execute_built_in(shell, cmd);
		free_and_exit(shell, 0);
    }
    else
    {
        if (find_full_path(shell, cmd->args[0]) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(cmd->args[0], STDERR_FILENO);
            ft_putstr_fd(": command not found\n", STDERR_FILENO);
            exit(127);
        }
        execve(shell->full_path, cmd->args, shell->env);
        perror("execve failed");
		free_and_exit(shell, 1);
    }
}

void 	preprocess_heredocs(t_command *cmd, t_shell *shell)
{
	t_redir	*hd;
	int		fd, expand;

	while (cmd)
	{
		fd = -1;
		hd = cmd->heredocs;
		while (hd)
		{
			expand = (hd->src_token && hd->src_token->quoted == 0);
			fd = handle_heredoc(hd->filename, shell, expand);
			if (fd == -1)
			{
				free_redirections(cmd->heredocs);
				cmd->heredocs = NULL;
				break;
			}
			hd = hd->next;
		}
		if (fd != -1)
		{
			free_redirections(cmd->heredocs);
			cmd->heredocs = NULL;

			if (cmd->infiles)
				free_redirections(cmd->infiles);
			
			cmd->infiles = malloc(sizeof(t_redir));
			if (!cmd->infiles)
				return;
			cmd->infiles->type = T_REDIRECT_IN;
			cmd->infiles->filename = ft_itoa(fd); // "4", etc.
			cmd->infiles->next = NULL;
			cmd->infiles->src_token = NULL;
		}
		cmd = cmd->next;
	}
}
