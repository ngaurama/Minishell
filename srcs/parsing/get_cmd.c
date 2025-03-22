/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:17:00 by npbk              #+#    #+#             */
/*   Updated: 2025/03/22 03:38:15 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void add_argument_to_cmd(t_command *cmd, char *arg, int *arg_count)
{
    if (*arg_count < MAX_ARGS - 1)
    {
        cmd->args[*arg_count] = ft_strdup(arg);
        (*arg_count)++;
        cmd->args[*arg_count] = NULL;
    }
}

void add_redirection(t_redir **redir_list, char *filename, int type)
{
    t_redir *new;
    t_redir *tmp;

    new = malloc(sizeof(t_redir));
    if (!new)
        return;
    new->filename = ft_strdup(filename);
    new->type = type;
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

void handle_redirection(t_command *cmd, t_arg *tokens)
{
    if (!tokens->next || tokens->next->type != T_WORD)
    {
        printf("Syntax error: missing filename after `%s`\n", tokens->value);
        return;
    }
    if (tokens->type == T_REDIRECT_IN)
        add_redirection(&cmd->infiles, tokens->next->value, tokens->type);
    else
        add_redirection(&cmd->outfiles, tokens->next->value, tokens->type);
}

t_command	*handle_pipe(t_command *cmd, int *arg_count, t_arg *prev_token)
{
	if (!prev_token || prev_token->type == T_PIPE)
	{
		printf("Syntax error: unexpected token `|`\n");
		return (NULL);
	}
	cmd->pipe = 1;
	cmd->next = init_command();
	*arg_count = 0;
	return (cmd->next);
}

t_command	*parse_tokens(t_arg *tokens)
{
	t_command	*cmd;
	t_command	*head;
	int			arg_count;
	t_arg		*prev_token;

	cmd = init_command();
	head = cmd;
	arg_count = 0;
	prev_token = NULL;
	while (tokens)
	{
		if (tokens->type == T_WORD && (!prev_token || (prev_token->type !=
			 T_REDIRECT_IN && prev_token->type != T_REDIRECT_OUT &&
			 prev_token->type != T_APPEND)))
			add_argument_to_cmd(cmd, tokens->value, &arg_count);
		else if (tokens->type == T_REDIRECT_IN || 
			tokens->type == T_REDIRECT_OUT || tokens->type == T_APPEND)
			handle_redirection(cmd, tokens);
		else if (tokens->type == T_PIPE && 
			(cmd = handle_pipe(cmd, &arg_count, prev_token)) == NULL)
			return (NULL);
		prev_token = tokens;
		tokens = tokens->next;
	}
	return (head);
}
