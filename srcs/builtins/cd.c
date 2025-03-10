/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:32:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 23:16:05 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void ft_cd(t_shell *shell)
{
    char *dir = shell->arguments->next ? shell->arguments->next->value : getenv("HOME");

    if (!dir)
    {
        fprintf(stderr, "cd: HOME not set\n");
        return;
    }
    char old_pwd[1024];
    if (!getcwd(old_pwd, sizeof(old_pwd)))
    {
        perror("cd failed");
        return;
    }
    if (chdir(dir) != 0)
    {
        perror("cd failed");
        return;
    }
    char new_pwd[1024];
    if (!getcwd(new_pwd, sizeof(new_pwd)))
    {
        perror("cd failed");
        return;
    }
    execute_export_single(shell, "OLDPWD", old_pwd);
    execute_export_single(shell, "PWD", new_pwd);
}

void execute_export_single(t_shell *shell, const char *key, const char *value)
{
    char *key_value = malloc(strlen(key) + strlen(value) + 2);
    if (!key_value)
        return;
    sprintf(key_value, "%s=%s", key, value);
    int found = 0;
    int i = 0;
    while (shell->env[i])
    {
        if (strncmp(shell->env[i], key, strlen(key)) == 0 && shell->env[i][strlen(key)] == '=')
        {
            free(shell->env[i]);
            shell->env[i] = key_value;
            found = 1;
            break;
        }
        i++;
    }
    if (!found)
    {
        int env_count = 0;
        while (shell->env[env_count])
            env_count++;

        char **new_env = malloc((env_count + 2) * sizeof(char *));
        if (!new_env)
        {
            free(key_value);
            return;
        }
        int j = -1;
        while (j++ < env_count)
            new_env[j] = shell->env[j];
        new_env[env_count] = key_value;
        new_env[env_count + 1] = NULL;
        free(shell->env);
        shell->env = new_env;
    }
}
