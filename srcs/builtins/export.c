/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 23:23:08 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_variables(t_shell *shell, const char *input)
{
    char *result = malloc(1);
    
    (void)shell;
    const char *ptr = input;
    while (*ptr)
    {
        if (*ptr == '$')
        {
            ptr++;
            if (*ptr == '{')
            {
                ptr++;
                const char *var_start = ptr;
                while (*ptr && *ptr != '}')
                    ptr++;
                if (*ptr == '}')
                {
                    char var_name[1024];
                    strncpy(var_name, var_start, ptr - var_start);
                    var_name[ptr - var_start] = '\0';
                    ptr++;
                    char *var_value = getenv(var_name);
                    if (var_value)
                    {
                        result = realloc(result, strlen(result) + strlen(var_value) + 1);
                        strcat(result, var_value);
                    }
                }
            }
            else
            {
                const char *var_start = ptr;
                while (*ptr && (isalnum(*ptr) || *ptr == '_'))
                    ptr++;

                char var_name[1024];
                strncpy(var_name, var_start, ptr - var_start);
                var_name[ptr - var_start] = '\0';
                char *var_value = getenv(var_name);
                if (var_value)
                {
                    result = realloc(result, strlen(result) + strlen(var_value) + 1);
                    strcat(result, var_value);
                }
            }
        }
        else
        {
            result = realloc(result, strlen(result) + 2);
            strncat(result, ptr, 1);
            ptr++;
        }
    }

    return result;
}

void ft_export(t_shell *shell)
{
    t_arg *current = shell->arguments->next;

    while (current)
    {
        char *key_value = current->value;
        char *equal_sign = strchr(key_value, '=');

        if (equal_sign)
        {
            *equal_sign = '\0';
            char *key = key_value;
            char *value = equal_sign + 1;
            int found = 0;
            int i = 0;
            while (shell->env[i])
            {
                if (strncmp(shell->env[i], key, strlen(key)) == 0 && shell->env[i][strlen(key)] == '=')
                {
                    free(shell->env[i]);
                    shell->env[i] = malloc(strlen(key) + strlen(value) + 2);
                    sprintf(shell->env[i], "%s=%s", key, value);
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
                shell->env = realloc(shell->env, (env_count + 2) * sizeof(char *));
                shell->env[env_count] = malloc(strlen(key) + strlen(value) + 2);
                sprintf(shell->env[env_count], "%s=%s", key, value);
                shell->env[env_count + 1] = NULL;
            }
        }
        current = current->next;
    }
}
