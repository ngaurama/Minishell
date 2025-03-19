/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:18:28 by npbk              #+#    #+#             */
/*   Updated: 2025/03/19 19:25:55 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_arg	*add_token(t_arg *head, char *token, int type)
{
	t_arg *new;
	t_arg *tmp;

	if (!token || *token == '\0')
		return (head);
	new = malloc(sizeof(t_arg));
	if (!new)
		return (NULL);
	new->value = ft_strdup(token);
	new->type = type;
	new->next = NULL;
	if (!head)
		return (new);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (head);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args[0] = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->append = 0;
	cmd->pipe = 0;
	cmd->next = NULL;
	return (cmd);
}
