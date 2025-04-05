/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/05 10:57:58 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	try_export(const char *key, const char *value, t_shell *shell,
		int *status)
{
	if (!is_valid_identifier(key))
	{
		print_error("export: not a valid identifier", key, value);
		*status = 1;
		return ;
	}
	set_env_var(shell, key, value);
}

static void	parse_export_arg(char *arg, char **key, char **value,
		int *append_mode)
{
	char	*op;

	*append_mode = 0;
	*value = NULL;
	op = ft_strstr(arg, "+=");
	if (op)
	{
		*append_mode = 1;
		*key = ft_substr(arg, 0, op - arg);
		*value = ft_strdup(op + 2);
	}
	else
	{
		op = ft_strchr(arg, '=');
		if (op)
		{
			*key = ft_substr(arg, 0, op - arg);
			*value = ft_strdup(op + 1);
		}
		else
			*key = ft_strdup(arg);
	}
}

static void	handle_append_export(t_shell *shell, char *key, char *value,
		int *status)
{
	char	*old_value;
	char	*new_value;

	old_value = get_env_value(shell->env, key);
	if (old_value)
		new_value = ft_strjoin(old_value, value);
	else
		new_value = ft_strdup(value);
	try_export(key, new_value, shell, status);
	free(new_value);
}

static void	export_var_from_arg(char *arg, t_shell *shell, int *status)
{
	char	*key;
	char	*value;
	int		append_mode;

	parse_export_arg(arg, &key, &value, &append_mode);
	if (append_mode)
		handle_append_export(shell, key, value, status);
	else
		try_export(key, value, shell, status);
	free(key);
	if (value)
		free(value);
}

void	ft_export(t_shell *shell)
{
	int	i;
	int	status;

	if (!shell->cmds->args[1])
	{
		print_sorted_env(shell);
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	status = 0;
	while (shell->cmds->args[i])
	{
		export_var_from_arg(shell->cmds->args[i], shell, &status);
		i++;
	}
	shell->exit_status = status;
}
