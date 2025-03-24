/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/24 01:38:02 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_env_as_export(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i])
	{
		printf("declare -x %s\n", shell->env[i]);
		i++;
	}
}

static void	try_export(const char *key, const char *value, t_shell *shell,
	int *status)
{
	if (!is_valid_identifier(key))
	{
		print_error("export: not a valid identifier");
		*status = 1;
		return ;
	}
	set_env_var(shell, key, value);
}

static void	export_var_from_arg(char *arg, t_shell *shell, int *status)
{
	char	*key;
	char	*value;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		key = ft_substr(arg, 0, eq - arg);
		value = ft_strdup(eq + 1);
		try_export(key, value, shell, status);
		free(key);
		free(value);
	}
	else
		try_export(arg, "", shell, status);
}

void		ft_export(t_shell *shell)
{
	int	i;
	int	status;

	if (!shell->cmds->args[1])
	{
		print_env_as_export(shell);
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
