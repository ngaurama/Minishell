/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 12:14:13 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_arg {
    char *value;
    struct s_arg *next;
} t_arg;

typedef struct s_shell {
    t_arg *arguments;
    char *input;
    char *command;
    char *full_path;
    pid_t pid;
    char **env;
} t_shell;

t_arg *add_argument(t_arg *head, char *value);
void free_arguments(t_arg *head);
int check_built_in(t_shell *shell);
int execute_command(t_shell *shell);
int find_full_path(t_shell *shell);
char	*ft_strtok(char *str, const char *delim);
char	*ft_strdup(const char *s1);

// init.c
void	init_shell(t_shell *shell, char **envp);
void	free_shell(t_shell *shell);

#endif