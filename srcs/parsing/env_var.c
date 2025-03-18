/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 18:55:08 by npbk              #+#    #+#             */
/*   Updated: 2025/03/18 17:54:33 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_variable_token(char *token, t_shell *shell)
{
	char	*var_start;
	char	*var_name;
	char	*var_value;
	char	*expanded;

	var_start = ft_strchr(token, '$');
	if (!var_start || !var_start[1])
		return (ft_strdup(token));
	var_name = extract_var_name(var_start + 1);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(shell->env, var_name);
	if (!var_value)
	{
		free(var_name);
		return (NULL);
	}
	expanded = create_expanded_token(token, var_start, var_value, var_name);
	free(var_name);
	free(var_value);
	return (expanded);
}

char	*expand_tilde(char *token, t_shell *shell)
{
	char	*home;
	char	*expanded;
	int		home_len;
	int		token_len;

	if (token[0] != '~')
		return (ft_strdup(token));
	home = get_env_value(shell->env, "HOME");
	if (!home || !home[0])
	{
		free(home);
		return (ft_strdup(token));
	}
	home_len = ft_strlen(home);
	token_len = ft_strlen(token);
	expanded = malloc(home_len + token_len);
	if (!expanded)
	{
		free(home);
		return (NULL);
	}
	ft_strlcpy(expanded, home, home_len + 1);
	ft_strlcat(expanded, token + 1, home_len + token_len);
	free(home);
	return (expanded);
}

void expand_token(t_tokenizer *tok, t_shell *shell, char **expanded_tilde, char **expanded_var)
{
	if (tok->quoted)
		*expanded_tilde = ft_strdup(tok->token);
	else
		*expanded_tilde = expand_tilde(tok->token, shell);
	if (tok->should_expand)
		*expanded_var = expand_variable_token(*expanded_tilde, shell);
	else
		*expanded_var = ft_strdup(*expanded_tilde);
}
