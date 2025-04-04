/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 12:10:05 by npbk              #+#    #+#             */
/*   Updated: 2025/04/04 22:06:27 by ngaurama         ###   ########.fr       */
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

static void	init_minimal_env(t_shell *shell, char **env)
{
	char	*env_value;
	char	*cwd;
	int		lvl;
	char	*next;

	cwd = getcwd(NULL, 0);
	env_value = get_env_value(env, "PWD");
	if (cwd && !env_value)
		set_env_var(shell, "PWD", cwd);
	free(cwd);
	free(env_value);
	env_value = get_env_value(env, "SHLVL");
	if (!env_value)
		set_env_var(shell, "SHLVL", "1");
	else
	{
		lvl = ft_atoi(env_value);
		next = ft_itoa(lvl + 1);
		set_env_var(shell, "SHLVL", next);
		free(next);
	}
	free(env_value);
}

void	init_shell(t_shell *shell, char **envp)
{
	char	*env_value;

	ft_memset(shell, 0, sizeof(t_shell));
	if (!envp)
	{
		shell->env = NULL;
		shell->exit_status = 0;
		return ;
	}
	init_env(shell, envp);
	init_minimal_env(shell, shell->env);
	env_value = get_env_value(shell->env, "OLDPWD");
	if (!env_value)
		set_env_var(shell, "OLDPWD", NULL);
	free(env_value);
}

void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell || shell->freed)
		return ;
	shell->freed = 1;
	if (shell->input)
		free(shell->input);
	if (shell->full_path)
		free(shell->full_path);
	if (shell->command)
		shell->command = NULL;
	if (shell->arguments)
		free_arguments(shell->arguments);
	if (shell->cmds)
		free_commands(shell->cmds);
	free(shell->current_dir);
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
			free(shell->env[i++]);
		free(shell->env);
	}
	int fd = 2;
	while (++fd < 1024)
		close(fd);
	rl_clear_history();
}
