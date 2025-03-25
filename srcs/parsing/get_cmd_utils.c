/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:37:25 by npbk              #+#    #+#             */
/*   Updated: 2025/03/25 17:43:23 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_standalone(t_arg *token, t_arg *prev)
{
	if (!token || token->type != T_WORD)
		return (0);
	if (!prev)
		return (1);
	return (prev->type != T_REDIRECT_IN && prev->type != T_REDIRECT_OUT &&
			prev->type != T_APPEND && prev->type != T_HEREDOC);
}

int	is_redir_token(int type)
{
	return (type == T_REDIRECT_IN || type == T_REDIRECT_OUT ||
			type == T_APPEND || type == T_HEREDOC);
}

int	handle_redir_or_free(t_command *cmd, t_arg **tokens,
		t_command *head)
{
	if (!handle_redirection(cmd, *tokens))
	{
		free_commands(head);
		return (0);
	}
	if ((*tokens)->next)
		*tokens = (*tokens)->next;
	return (1);
}
