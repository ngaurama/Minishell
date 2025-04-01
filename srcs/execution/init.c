/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:10:05 by npbk              #+#    #+#             */
/*   Updated: 2025/04/01 13:58:04 by npagnon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_env(t_shell *shell, char **envp)
{
	int	i;
	int	envp_size;

	i = 0;
	envp_size = 0;
	while (envp[envp_size])
		envp_size++;
	shell->env_cap = envp_size + ENV_EXTRA;
	shell->env = malloc(sizeof(char *) * shell->env_cap);
	if (!shell->env)
	{
		perror("malloc failed");
		exit(1);
	}
	i = -1;
	while (envp[++i])
		shell->env[i] = ft_strdup(envp[i]);
	shell->env[i] = NULL;
	shell->current_dir = getcwd(NULL, 0);
	if (!shell->current_dir)
		shell->current_dir = ft_strdup(get_env_value(shell->env, "PWD"));
	if (!shell->current_dir)
		shell->current_dir = ft_strdup("/");
}

void	init_shell(t_shell *shell, char **envp)
{
	ft_memset(shell, 0, sizeof(t_shell));
	if (!envp)
	{
		shell->env = NULL;
		shell->exit_status = 0;
		return ;
	}
	init_env(shell, envp);
	shell->exit_status = 0;
	shell->redir_err = 0;
}

void	free_shell(t_shell *shell)
{
	int	i;

	if (shell->input)
		free(shell->input);
	if (shell->full_path)
		free(shell->full_path);
	if (shell->command)
		shell->command = NULL;
	if (shell->arguments)
	{
		free_arguments(shell->arguments);
	}
	if (shell->cmds)
	{
		free_commands(shell->cmds);
	}
	free(shell->current_dir);
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	clear_history(); //mac
	//rl_clear_history();
}
