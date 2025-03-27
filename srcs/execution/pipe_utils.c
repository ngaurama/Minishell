/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 02:53:58 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/27 14:15:05 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void setup_child_pipes(int prev_pipe_read, int pipefd[2], t_command *cmd)
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

void execute_child_pipes(t_shell *shell, t_command *cmd)
{
    if (redirection(cmd, shell) != 0)
        exit(1);
    if (check_built_in(cmd))
    {
        execute_built_in(shell, cmd);
        exit(0);
    }
    else
    {
        if (find_full_path(shell, cmd->args[0]) != 0)
        {
            ft_putstr_fd("minishell: command not found: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd("\n", 2);
            exit(127);
        }
        execve(shell->full_path, cmd->args, shell->env);
        perror("execve failed");
        exit(1);
    }
}

void preprocess_heredocs(t_command *cmd, t_shell *shell)
{
	int	expand;
	int	fd;
	
    while (cmd)
    {
        if (cmd->heredocs)
        {
			expand = (cmd->heredocs->src_token->quoted == 0);
            fd = handle_heredoc(cmd->heredocs->filename, shell, expand);
            if (fd != -1)
            {
                cmd->infiles = malloc(sizeof(t_redir));
                cmd->infiles->type = T_REDIRECT_IN;
                cmd->infiles->filename = ft_itoa(fd);
                cmd->infiles->next = NULL;
                cmd->heredocs = NULL;
            }
        }
        cmd = cmd->next;
    }
}
