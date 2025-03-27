/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/27 23:45:58 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	sort_env(char **env, int size)
{
    int     i;
    int     j;
    char    *tmp;
    int     swapped;

    i = 0;
    while (i < size - 1)
    {
        swapped = 0;
        j = 0;
        while (j < size - i - 1)
        {
            if (ft_strcmp(env[j], env[j + 1]) > 0)
            {
                tmp = env[j];
                env[j] = env[j + 1];
                env[j + 1] = tmp;
                swapped = 1;
            }
            j++;
        }
        if (!swapped)
            break;
        i++;
    }
}

static void	print_env_var(char *var)
{
    char *equal_sign = ft_strchr(var, '=');
    
    if (equal_sign && *(equal_sign + 1))
    {
        *equal_sign = '\0';
        printf("declare -x %s=\"%s\"\n", var, equal_sign + 1);
        *equal_sign = '=';
    }
    else
        printf("declare -x %s\n", var);
}

static void	print_sorted_env(t_shell *shell)
{
    int     count = 0;
    char    **env_copy;
    int     i;

    while (shell->env[count]) 
		count++;
    env_copy = malloc(sizeof(char *) * (count + 1));
    if (!env_copy) 
		return;
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
	{
		if (get_env_value(shell->env, arg))
			try_export(arg, get_env_value(shell->env, arg), shell, status);
		else
			try_export(arg, NULL, shell, status);
	}
}

void		ft_export(t_shell *shell)
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
