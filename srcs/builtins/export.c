/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 22:14:54 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_env_var(char *var)
{
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	if (equal_sign && *(equal_sign + 1) && *(equal_sign - 1) == '_')
		return ;
	else if (equal_sign && *(equal_sign + 1))
	{
		*equal_sign = '\0';
		printf("declare -x %s=\"%s\"\n", var, equal_sign + 1);
		*equal_sign = '=';
	}
	else
		printf("%s\n", var);
}

static void	print_sorted_env(t_shell *shell)
{
	int		count;
	char	**env_copy;
	int		i;

	count = 0;
	while (shell->env[count])
		count++;
	env_copy = malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return ;
	i = -1;
	while (++i < count)
		env_copy[i] = shell->env[i];
	env_copy[count] = NULL;
	sort_env(env_copy, count);
	i = -1;
	while (env_copy[++i])
		print_env_var(env_copy[i]);
	free(env_copy);
}

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

static void	export_var_from_arg(char *arg, t_shell *shell, int *status)
{
	char	*key;
	char	*value;
	char	*op;
	int		append_mode;
	char	*old_value;
	char	*new_value;

	append_mode = 0;
	op = ft_strstr(arg, "+=");
	if (op)
	{
		append_mode = 1;
		key = ft_substr(arg, 0, op - arg);
		value = ft_strdup(op + 2);
	}
	else
	{
		op = ft_strchr(arg, '=');
		if (op)
		{
			key = ft_substr(arg, 0, op - arg);
			value = ft_strdup(op + 1);
		}
		else
		{
			key = ft_strdup(arg);
			value = NULL;
		}
	}
	if (append_mode)
	{
		old_value = get_env_value(shell->env, key);
		if (old_value)
			new_value = ft_strjoin(old_value, value);
		else
			new_value = ft_strdup(value);
		try_export(key, new_value, shell, status);
		free(new_value);
	}
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
