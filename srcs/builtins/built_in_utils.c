/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:21:16 by npbk              #+#    #+#             */
/*   Updated: 2025/04/03 12:30:18 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_cd_error(char *dir, t_shell *shell)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("minishell: cd: ", dir);
	msg = ft_strjoin(tmp, ": No such file or directory\n");
	free(tmp);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
	shell->exit_status = 1;
}

int	validate_cd_args(t_shell *shell)
{
	int	count;

	count = 0;
	while (shell->cmds->args[count])
		count++;
	if (count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

void	sort_env(char **env, int size)
{
	int		i;
	int		j;
	char	*tmp;
	int		swapped;

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
			break ;
		i++;
	}
}
