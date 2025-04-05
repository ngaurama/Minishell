/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npagnon <npagnon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:55:12 by npagnon           #+#    #+#             */
/*   Updated: 2025/04/05 10:57:22 by npagnon          ###   ########.fr       */
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

void	print_sorted_env(t_shell *shell)
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
