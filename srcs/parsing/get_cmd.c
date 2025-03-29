/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:17:00 by npbk              #+#    #+#             */
/*   Updated: 2025/03/29 21:11:23 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_redirection(t_redir **redir_list, t_arg *token, int type)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->filename = ft_strdup(token->value);
	new->type = type;
	new->src_token = token;
	new->next = NULL;

	if (!*redir_list)
		*redir_list = new;
	else
	{
		tmp = *redir_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	handle_redirection(t_command *cmd, t_arg *tokens)
{
	t_arg	*next;

	next = tokens->next;
	if (!next)
	{
		print_parse_error("\n");
		return (0);
	}
	if (next->type != T_WORD)
	{
		print_parse_error(next->value);
		return (0);
	}
	if (tokens->type == T_REDIRECT_IN)
		add_redirection(&cmd->infiles, next, tokens->type);
	else if (tokens->type == T_HEREDOC)
		add_redirection(&cmd->heredocs, next, tokens->type);
	else
		add_redirection(&cmd->outfiles, next, tokens->type);
	return (1);
}

int	handle_pipe(t_command **cmd, int *arg_count, t_arg *prev_token)
{
	if (!prev_token || prev_token->type == T_PIPE)
	{
		print_parse_error("|");
		(*cmd)->next = NULL;
		return (1);
	}
	(*cmd)->pipe = 1;
	(*cmd)->next = init_command();
	if (!(*cmd)->next)
		return (1);
	*arg_count = 0;
	*cmd = (*cmd)->next;
	return (0);
}

int	handle_token(t_shell *shell, t_parse_data *p_data, t_arg **tokens)
{
	if (is_standalone(*tokens, p_data->prev_token))
	{
		add_argument_to_cmd(shell, p_data->cmd, (*tokens)->value,
			&p_data->arg_count);
	}
	else if (is_redir_token((*tokens)->type))
	{
		if (!handle_redir_or_free(p_data->cmd, tokens, p_data->head))
			return (1);
	}
	else if ((*tokens)->type == T_PIPE &&
		handle_pipe(&p_data->cmd, &p_data->arg_count, p_data->prev_token))
		return (1);
	return (0);
}

t_command	*parse_tokens(t_shell *shell, t_arg *tokens)
{
	t_parse_data	p_data;

	p_data.cmd = init_command();
	p_data.head = p_data.cmd;
	p_data.prev_token = NULL;
	p_data.arg_count = 0;
	while (tokens)
	{
		if (handle_token(shell, &p_data, &tokens))
		{
			free_commands(p_data.head);
			return (NULL);
		}
		p_data.prev_token = tokens;
		tokens = tokens->next;
	}
	if (p_data.prev_token && p_data.prev_token->type == T_PIPE)
	{
		print_parse_error("|");
		free_commands(p_data.head);
		return (NULL);
	}
	return (p_data.head);
}
