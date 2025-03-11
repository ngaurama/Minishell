/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 22:33:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/11 14:27:50 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *expand_variables(t_shell *shell, const char *input)
{
    (void)shell;
    char *result = malloc(1);
    if (!result)
        return NULL;
    result[0] = '\0';

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
                    ft_strncpy(var_name, var_start, ptr - var_start);
                    var_name[ptr - var_start] = '\0';
                    ptr++;
                    char *var_value = ft_getenv(shell, var_name);
                    if (var_value)
                    {
                        size_t result_len = ft_strlen(result);
                        size_t var_len = ft_strlen(var_value);
                        size_t new_len = result_len + var_len + 1;

                        char *new_result = malloc(new_len);
                        if (!new_result)
                        {
                            free(result);
                            return NULL;
                        }
                        ft_strcpy(new_result, result);
                        ft_strlcat(new_result, var_value, sizeof(new_result));
                        free(result);
                        result = new_result;
                    }
                }
            }
            else
            {
                const char *var_start = ptr;
                while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
                    ptr++;
                char var_name[1024];
                ft_strncpy(var_name, var_start, ptr - var_start);
                var_name[ptr - var_start] = '\0';
                char *var_value = ft_getenv(shell, var_name);
                if (var_value)
                {
                    size_t result_len = ft_strlen(result);
                    size_t var_len = ft_strlen(var_value);
                    size_t new_len = result_len + var_len + 1;
                    char *new_result = malloc(new_len);
                    if (!new_result)
                    {
                        free(result);
                        return NULL;
                    }
                    ft_strcpy(new_result, result);
                    ft_strlcat(new_result, var_value, sizeof(new_result));
                    free(result);
                    result = new_result;
                }
            }
        }
        else
        {
            size_t result_len = ft_strlen(result);
            size_t new_len = result_len + 2;
            char *new_result = malloc(new_len);
            if (!new_result)
            {
                free(result);
                return NULL;
            }
            ft_strcpy(new_result, result);
            new_result[result_len] = *ptr;
            new_result[result_len + 1] = '\0';
            free(result);
            result = new_result;
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
        char *equal_sign = ft_strchr(key_value, '=');

        if (equal_sign)
        {
            *equal_sign = '\0';
            char *key = key_value;
            char *value = equal_sign + 1;
            int found = 0;
            int i = 0;
            while (shell->env[i])
            {
                if (ft_strncmp(shell->env[i], key, ft_strlen(key)) == 0 && shell->env[i][ft_strlen(key)] == '=')
                {
                    free(shell->env[i]);
                    shell->env[i] = malloc(ft_strlen(key) + ft_strlen(value) + 2);
                    if (!shell->env[i])
                        return;
                    ft_strcpy(shell->env[i], key);
                    ft_strlcat(shell->env[i], "=", ft_strlen(key) + 2);
                    ft_strlcat(shell->env[i], value, ft_strlen(key) + ft_strlen(value) + 2);
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
                    return;
                int i = -1;
                while (i++ < env_count)
                    new_env[i] = shell->env[i];
                new_env[env_count] = malloc(ft_strlen(key) + ft_strlen(value) + 2);
                if (!new_env[env_count])
                {
                    free(new_env);
                    return;
                }
                ft_strcpy(new_env[env_count], key);
                ft_strlcat(new_env[env_count], "=", ft_strlen(key) + 2);
                ft_strlcat(new_env[env_count], value, ft_strlen(key) + ft_strlen(value) + 2);
                new_env[env_count + 1] = NULL;
                free(shell->env);
                shell->env = new_env;
            }
            
        }
        current = current->next;
    }
}
