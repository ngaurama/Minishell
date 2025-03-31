/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:37:25 by npbk              #+#    #+#             */
/*   Updated: 2025/03/31 19:02:07 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_standalone(t_arg *token, t_arg *prev)
{
	if (!token || token->type != T_WORD)
		return (0);
	if (!prev)
		return (1);
	return (prev->type != T_REDIRECT_IN && prev->type != T_REDIRECT_OUT && \
		prev->type != T_APPEND && prev->type != T_HEREDOC);
}

int	handle_redir_or_free(t_command *cmd, t_arg **tokens)
{
	if (!handle_redirection(cmd, *tokens))
		return (0);
	if ((*tokens)->next)
		*tokens = (*tokens)->next;
	return (1);
}

void	handle_strdup_failure(char **args, int i, t_shell *shell)
{
	perror("ft_strdup failed");
	while (--i >= 0)
		args[i] = NULL;
	free(args);
	free_shell(shell);
	exit(1);
}

void	default_redir_cmds(t_shell *shell, t_command *head)
{
	t_command	*current;
	int			temp_count;

	current = head;
	while (current)
	{
		if (!current->args)
		{
			temp_count = 0;
			if (current->heredocs && !current->pipe && !current->next)
				add_argument_to_cmd(shell, current, "cat", &temp_count);
			else if (current->infiles || current->outfiles || current->append)
				add_argument_to_cmd(shell, current, "true", &temp_count);
		}
		current = current->next;
	}
}

void	add_argument_to_cmd(t_shell *shell, t_command *cmd, char *arg,
			int *arg_count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (*arg_count + 2));
	if (!new_args)
	{
		perror("malloc failed");
		free_shell(shell);
		exit(1);
	}
	i = 0;
	while (i < *arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
		handle_strdup_failure(new_args, i, shell);
	new_args[i + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	(*arg_count)++;
}
