/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:45:23 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 13:52:30 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_home_path(char *prefix, t_shell *shell)
{
	if (!prefix || !*prefix)
		return (get_env_value(shell->env, "HOME"));
	if (ft_strcmp(prefix, "+") == 0)
		return (get_env_value(shell->env, "PWD"));
	if (ft_strcmp(prefix, "-") == 0)
		return (get_env_value(shell->env, "OLDPWD"));
	return (NULL);
}

static int	handle_special_tilde(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*prefix;
	char	*home;

	prefix = ft_substr(input, tok->i, 1);
	tok->i++;
	home = get_home_path(prefix, shell);
	free(prefix);
	if (!home)
		return (0);
	append_str_to_token(tok, home);
	free(home);
	return (1);
}

static int	handle_tilde_slash(t_tokenizer *tok, t_shell *shell)
{
	char	*home;

	home = get_home_path(NULL, shell);
	if (home)
	{
		append_str_to_token(tok, home);
		append_char_to_token(tok, '/');
		free(home);
	}
	else
		append_str_to_token(tok, "~/");
	tok->i++;
	return (1);
}

static int	handle_unexpanded_tilde(char *input, t_tokenizer *tok, int start)
{
	int		end;
	char	*unexpanded;

	end = tok->i;
	while (input[end] && !ft_isspace(input[end]))
		end++;
	unexpanded = ft_substr(input, start, end - start);
	append_str_to_token(tok, unexpanded);
	free(unexpanded);
	tok->i = end;
	return (1);
}

int	handle_tilde(char *input, t_tokenizer *tok, t_shell *shell)
{
	char	*home;
	int		start;

	start = tok->i;
	if (input[start] != '~' || tok->in_quotes)
		return (0);
	if (start > 0 && input[start - 1] != '=' && !ft_isspace(input[start - 1]))
		return (0);
	tok->i++;
	if (input[tok->i] == '+' || input[tok->i] == '-')
		return (handle_special_tilde(input, tok, shell));
	if (input[tok->i] == '/')
		return (handle_tilde_slash(tok, shell));
	if (input[tok->i] != '\0' && !(start > 0 && input[start - 1] == '='))
		return (handle_unexpanded_tilde(input, tok, start));
	home = get_home_path(NULL, shell);
	if (home)
	{
		append_str_to_token(tok, home);
		free(home);
		return (1);
	}
	append_char_to_token(tok, '~');
	return (0);
}
