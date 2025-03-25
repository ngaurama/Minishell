/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:03:53 by npbk              #+#    #+#             */
/*   Updated: 2025/03/25 17:43:33 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_value(char **env, char *var_name)
{
	int		i;
	int		len;
	char	*env_var;

	i = 0;
	len = ft_strlen(var_name);
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, len) && env[i][len] == '=')
		{
			env_var = ft_strdup(env[i] + len + 1);
			return (env_var);
		}
		i++;
	}
	return (NULL);
}

char	*extract_var_name(char *str)
{
	int		len;
	char	*var_name;

	len = 0;
	if (str[len] == '?')
		len++;
	else
	{
		while (ft_isalnum(str[len]) || str[len] == '_')
			len++;
	}
	if (len == 0)
		return (NULL);
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, str, len);
	var_name[len] = '\0';
	return (var_name);
}

char 	*expand_var(char *var, t_shell *shell)
{
	if (ft_strncmp(var, "?", 2) == 0)
		return ft_itoa(shell->exit_status);
	return (get_env_value(shell->env, var));
}

int		is_valid_var_start(char c)
{
	if (ft_isalpha(c) || c == '_' || c == '?')
		return (1);
	return (0);
}

int		handle_dollar(char *input, t_tokenizer *tok, t_shell *shell,
			int *in_quotes, char quote_char)
{
	char	next;

	if (input[tok->i] != '$')
		return (0);
	next = input[tok->i + 1];
	if (!should_expand_dollar(next, *in_quotes, quote_char))
	{
		append_char_to_token(tok, '$');
		tok->i++;
	}
	else
		expand_variable(input, tok, shell);
	return (1);
}
