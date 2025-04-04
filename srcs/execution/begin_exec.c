/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 20:31:09 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/04 22:11:44 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**realloc_env(char **env, int new_size)
{
	char	**new_env;
	int		i;

	i = 0;
	new_env = malloc(sizeof(char *) * (new_size + 1));
	if (!new_env)
		return (NULL);
	while (env[i] && i < new_size)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[new_size] = NULL;
	free(env);
	return (new_env);
}

void	update_env_underscore(t_shell *shell, char *cmd_name)
{
	char	*new_env_value;
	int		i;

	i = 0;
	new_env_value = ft_strjoin("_=", cmd_name);
	if (!new_env_value)
		return ;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], "_=", 2) == 0)
		{
			free(shell->env[i]);
			shell->env[i] = new_env_value;
			return ;
		}
		i++;
	}
	shell->env = realloc_env(shell->env, i + 1);
	if (shell->env)
	{
		shell->env[i] = new_env_value;
		shell->env[i + 1] = NULL;
	}
	else
		free(new_env_value);
}

void	execution(t_shell *shell)
{
	if (!shell || !shell->cmds)
	{
		shell->exit_status = 1;
		return ;
	}
	update_env_underscore(shell, shell->cmds->args[0]);
	if (shell->cmds->pipe)
		pipeline(shell);
	else if (check_built_in(shell->cmds))
		execute_built_in(shell, shell->cmds);
	else
		execute_command(shell);
}
