/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/18 17:56:58 by npbk             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <ctype.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#include <fcntl.h>

# define TOKEN_SIZE 256
# define MAX_ARGS 256

# define T_WORD          1  // Regular arguments
# define T_PIPE          2  // "|"
# define T_REDIRECT_IN   3  // "<"
# define T_REDIRECT_OUT  4  // ">"
# define T_HEREDOC       5  // "<<"
# define T_APPEND        6  // ">>"

typedef struct s_tokenizer
{
	int		i;
	int		j;
	int		type;
	int		should_expand;
	int		in_quotes;
	int		quoted;
	char	token[TOKEN_SIZE];
}	t_tokenizer;

typedef struct s_redir
{
    char            *filename;
    int             type;
    struct s_redir  *next;
} t_redir;

typedef struct s_arg
{
    char            *value;
    int             type;
    int             quoted;
    struct s_arg    *next;
}   t_arg;

typedef struct s_command
{
    char                *args[MAX_ARGS];   // Arguments for execve()
    t_redir             *infiles;          // Files for "<"
    t_redir             *outfiles;         // Files for ">" or ">>"
    int                 append;            // 1 if ">>", 0 if ">"
    int                 pipe;              // 1 if there's a pipe "|"
    struct s_command    *next;    // Next command in the pipeline
}   t_command;

typedef struct s_shell {
    t_arg       *arguments;
    t_command   *cmds;
    char        *input;
    char        *command;
    char        *full_path;
    pid_t       pid;
    char        **env;
} t_shell;

// add_arg.c
t_arg       *add_argument(t_arg *head, char *value);
void        free_arguments(t_arg *head);

// init.c
void	    init_shell(t_shell *shell, char **envp);
void	    free_shell(t_shell *shell);

// get_tokens.c
t_arg	    *tokenize_input(char *input, t_shell *shell);

// get_cmd.c
t_command	*parse_tokens(t_arg *tokens);

// env_var.c
char	    *expand_variable_token(char *token, t_shell *shell);
char	    *expand_tilde(char *token, t_shell *shell);
void        expand_token(t_tokenizer *tok, t_shell *shell,
     char **expanded_tilde, char **expanded_var);

// env_var_utils.c
char	    *get_env_value(char **env, char *var_name);
char	    *extract_var_name(char *start);
char	    *create_expanded_token(char *token, char *var_start,
     char *var_value, char *var_name);

// parse_init.c
t_arg	    *add_token(t_arg *head, char *token, int type);
void        free_tokens(t_arg *tokens);
t_command	*init_command(void);
void        free_commands(t_command *cmds);

// execute.c
int         check_built_in(t_shell *shell);
int         execute_command(t_shell *shell);
int         find_full_path(t_shell *shell, const char *command);
// utils.c
char	    *ft_strtok(char *str, const char *delim);
int	        ft_strcmp(const char *s1, const char *s2);
char	    *ft_strcat(char *dest, const char *src);
char	    *ft_strncpy(char *dest, const char *src, size_t n);
char	    *ft_strcpy(char *dest, const char *src);

//redirection.c
void redirection(t_shell *shell);

//pipe.c
void pipeline(t_shell *shell);

// BUILTINS
// built_in.c
void				execute_built_in(t_shell *shell);
// cd.c
void				ft_cd(t_shell *shell);
void				execute_export_single(t_shell *shell, const char *key,
						const char *value);
// echo.c
void				ft_echo(t_shell *shell);
// env.c
char				*ft_getenv(t_shell *shell, const char *name);
void				ft_env(t_shell *shell);
// export.c
void				ft_export(t_shell *shell);
// pwd.c
void				ft_pwd(t_shell *shell);
// unset.c
void				ft_unset(t_shell *shell);
// expand_variable.c
char				*expand_variables(t_shell *shell, const char *input);
char				*ft_getenv(t_shell *shell, const char *var_name);
char				*append_to_result(char *result, const char *str);
char				*extract_variable_name(const char **ptr);
char				*handle_variable_expansion(t_shell *shell, char *result,
						const char **ptr);
char				*handle_normal_text(char *result, const char **ptr);

// For debugging
void				print_path(void);
void print_token_list(t_arg *tokens);
void print_command_list(t_command *cmd);
void print_redirections(t_redir *redirs, char *type);

#endif