/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/10 17:52:53 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# define TOKEN_SIZE 256

# define T_WORD          1  // Regular arguments
# define T_PIPE          2  // "|"
# define T_REDIRECT_IN   3  // "<"
# define T_REDIRECT_OUT  4  // ">"
# define T_APPEND        5  // ">>"
# define T_HEREDOC       6  // "<<"

typedef struct s_arg
{
    char    *value;
    int     type;
    struct s_arg *next;
}   t_arg;


typedef struct s_shell {
    t_arg *arguments;
    char *input;
    char *command;
    char *full_path;
    pid_t pid;
    char **env;
} t_shell;

// add_arg.c
t_arg   *add_argument(t_arg *head, char *value);
t_arg	*add_token(t_arg *head, char *token, int type);
void    free_arguments(t_arg *head);

// init.c
void	init_shell(t_shell *shell, char **envp);
void	free_shell(t_shell *shell);

// get_tokens.c
t_arg	*tokenize_input(char *input);

// execute.c
int     check_built_in(t_shell *shell);
int     execute_command(t_shell *shell);
int     find_full_path(t_shell *shell);

// utils.c
char	*ft_strtok(char *str, const char *delim);
char	*ft_strdup(const char *s1);

#endif