/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:37:25 by npbk              #+#    #+#             */
/*   Updated: 2025/03/29 22:05:38 by npbk             ###   ########.fr       */
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

int	is_redir_token(int type)
{
	return (type == T_REDIRECT_IN || type == T_REDIRECT_OUT || \
		type == T_APPEND || type == T_HEREDOC);
}

int	handle_redir_or_free(t_command *cmd, t_arg **tokens, t_arg *prev_token)
{
	if (!handle_redirection(cmd, *tokens, prev_token))
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
