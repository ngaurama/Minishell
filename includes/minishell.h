/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npbk <npbk@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/03/22 03:25:18 by npbk             ###   ########.fr       */
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
#include <signal.h>

# define MAX_ARGS 256

#include <signal.h>

extern volatile sig_atomic_t g_signal_num;


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
	char	*token;
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
    int         exit_status;
} t_shell;

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
t_command	*init_command(void);

// free_parse.c
void	    free_expanded_tokens(char *expanded_tilde, char *expanded_var,
    t_tokenizer *tok);
void        free_commands(t_command *cmds);
void        free_tokens(t_arg *tokens);

// parse_utils.c
void	    append_str_to_token(t_tokenizer *tok, char *str);
int	        is_space_or_meta(char c);
void	    tok_reset(t_tokenizer *tok);
int	handle_quoted_var(char *input, t_tokenizer *tok, t_shell *shell);
    
// execute.c
void update_exit_status(t_shell *shell, int status);
int check_built_in(t_command *cmds);
int         execute_command(t_shell *shell);
int         find_full_path(t_shell *shell, const char *command);
void restore_fds(int saved_stdin, int saved_stdout);
// utils.c
char	    *ft_strtok(char *str, const char *delim);
int	        ft_strcmp(const char *s1, const char *s2);
char	    *ft_strcat(char *dest, const char *src);
char	    *ft_strncpy(char *dest, const char *src, size_t n);
char	    *ft_strcpy(char *dest, const char *src);
void        free_arguments(t_arg *head);

//redirection.c
int redirection(t_shell *shell);

//pipe.c
void pipeline(t_shell *shell);

// BUILTINS
// built_in.c
void				execute_built_in(t_shell *shell);
// cd.c
void				ft_cd(t_shell *shell);

// echo.c
void				ft_echo(t_shell *shell);
// env.c
void				ft_env(t_shell *shell);
// export.c
void				ft_export(t_shell *shell);
// pwd.c
void				ft_pwd(t_shell *shell);
// unset.c
void				ft_unset(t_shell *shell);

//set_get_unset.c
void set_env_var(t_shell *shell, const char *key, const char *value);
void unset_env_var(t_shell *shell, const char *key);

// For debugging
void print_token_list(t_arg *tokens);
void print_command_list(t_command *cmd);
void print_redirections(t_redir *redirs, char *type);

#endif