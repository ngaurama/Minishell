/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:29:19 by ngaurama          #+#    #+#             */
/*   Updated: 2025/04/03 20:35:35 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _XOPEN_SOURCE 700

# include "../libft/libft.h"
# include "../libft/get_next_line.h"
# include <ctype.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <sys/stat.h>

# define ENV_EXTRA 1024

extern volatile sig_atomic_t	g_signal_num;

# define T_WORD			1	// Regular arguments
# define T_PIPE			2	// "|"
# define T_REDIRECT_IN	3	// "<"
# define T_REDIRECT_OUT	4	// ">"
# define T_HEREDOC		5	// "<<"
# define T_APPEND		6	// ">>"

typedef struct s_tokenizer
{
	int		i;
	int		j;
	int		type;
	int		should_expand;
	int		heredoc_next;
	int		in_quotes;
	int		quoted;
	char	quote_char;
	int		token_capacity;
	char	*token;
}	t_tokenizer;

typedef struct s_arg
{
	char			*value;
	int				type;
	int				quoted;
	struct s_arg	*next;
}	t_arg;

typedef struct s_redir
{
	char			*filename;
	int				type;
	t_arg			*src_token;
	struct s_redir	*next;
	int				fd;
}	t_redir;

typedef struct s_command
{
	char				**args;				// Arguments for execve()
	t_redir				*infiles;			// Files for "<"
	t_redir				*outfiles;			// Files for ">" or ">>"
	t_redir				*heredocs;
	int					append;				// 1 if ">>", 0 if ">"
	int					pipe;				// 1 if there's a pipe "|"
	int					pipefd[2];
	struct s_command	*next;				// Next command in the pipeline
}	t_command;

typedef struct s_parse_data
{
	t_command	*cmd;
	t_command	*head;
	t_arg		*prev_token;
	int			arg_count;
}	t_parse_data;

typedef struct s_shell
{
	t_arg		*arguments;
	t_command	*cmds;
	char		*input;
	char		*command;
	char		*full_path;
	pid_t		pid;
	char		**env;
	int			env_cap;
	int			exit_status;
	char		*current_dir;
	int			redir_err;
	int			err_printed;
	int			freed;
}	t_shell;

// init.c
void		init_shell(t_shell *shell, char **envp);
void		free_shell(t_shell *shell);

// get_tokens.c / get_tokens_2.c
t_arg		*tokenize_input(char *input, t_shell *shell);
int			handle_quotes(char *input, t_tokenizer *tok,
				int in_word, t_shell *shell);

// env_var.c
int			handle_quoted_var(char *input, t_tokenizer *tok, t_shell *shell);
int			handle_tilde(char *input, t_tokenizer *tok, t_shell *shell);
int			handle_dollar(char *input, t_tokenizer *tok, t_shell *shell);
void		expand_variable(char *input, t_tokenizer *tok, t_shell *shell,
				int skip);
int			handle_quote_state(char *input, t_tokenizer *tok,
				int *in_quotes, char *quote_char);

// env_var_utils.c
char		*get_env_value(char **env, char *var_name);
char		*extract_var_name(const char *str);
int			is_valid_var_start(char c);
char		*expand_var(char *var, t_shell *shell);
int			should_expand_dollar(char next, int in_quotes, char quote_char);

// handle_dollar.c
int			handle_single_dollar(t_tokenizer *tok, int quoted);
int			handle_exit_expansion(t_tokenizer *tok, t_shell *shell, int quoted);
int			skip_digits(char *input, t_tokenizer *tok, int quoted);
int			handle_dollar_quote(char *input, t_tokenizer *tok, t_shell *shell);
int			handle_literal_dollar(t_tokenizer *tok);

// get_cmd.c / get_cmd_utils.c
t_command	*parse_tokens(t_shell *shell, t_arg *tokens);
int			handle_redirection(t_command *cmd, t_arg *tokens);
int			is_standalone(t_arg *token, t_arg *prev);
void		default_redir_cmds(t_shell *shell, t_command *head);
int			handle_redir_or_free(t_command *cmd, t_arg **tokens);
void		add_argument_to_cmd(t_shell *shell, t_command *cmd, char *arg,
				int *arg_count);

// parse_init.c
t_arg		*add_token(t_arg *head, char *token, int type, int quoted);
t_command	*init_command(void);
int			init_tokenizer(t_tokenizer *tok);
int			ensure_token_capacity(t_tokenizer *tok, int extra);
void		tok_reset(t_tokenizer *tok);

// free_parse.c
void		free_redirections(t_redir *redirs);
void		free_commands(t_command *cmds);
void		free_arguments(t_arg *args);
void		free_tokenizer(t_tokenizer *tok);

// parse_utils.c
void		append_char_to_token(t_tokenizer *tok, char c);
void		append_str_to_token(t_tokenizer *tok, char *str);
int			is_space_or_meta(char c);
int			skip_whitespace(char *input, t_tokenizer *tok);
void		print_parse_error(const char *token);

// execute.c
int			execute_command(t_shell *shell);

// execute_utils.c
void		update_exit_status(t_shell *shell, int status);
int			save_fds(int *saved_stdin, int *saved_stdout);
void		restore_fds(int saved_stdin, int saved_stdout);
void		handle_command_not_found(t_shell *shell);
void		execute_child_process(t_shell *shell);

// full_path.c
int			find_full_path(t_shell *shell, const char *command);

// utils.c
void		print_error(const char *msg);
char		*ft_strtok(char *str, const char *delim);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strncpy(char *dest, const char *src, size_t n);
char		*ft_strcpy(char *dest, const char *src);

// utils2.c
void		child_error_and_exit(t_shell *shell, const char *cmd);
void	read_and_write_stderr(int fd);

//redirection.c
// int			redirection(t_shell *shell);
int			handle_heredoc(const char *delimiter, t_shell *shell, int expand);
int			redirection(t_command *cmd, t_shell *shell);

//redirection_utils.c
int			handle_redirect_in_file(const char *filename);
int			handle_heredoc_input(t_redir *redir, t_shell *shell);
int			stop_heredoc(char *line, const char *delimiter);
void		write_heredoc_line(int fd, char *line, t_shell *shell, int expand);
int			handle_inredir_error(t_redir *redir, t_shell *shell);

//heredoc_expand.c
char		*heredoc_expand(char *line, t_shell *shell);

// heredocs.c
void		preprocess_heredocs(t_command *cmd, t_shell *shell);

//pipe.c
void		pipeline(t_shell *shell);

//pipe_utils.c
void		setup_child_pipes(int prev_pipe_read, int pipefd[2],
				t_command *cmd);
void		pipeline_init(t_shell *shell, int buffer[2], int *prev_pipe_read);
void		execute_child_pipes(t_shell *shell, t_command *cmd);
void		free_and_exit(t_shell *shell, int exit_code);
void		handle_fork_error(int buffer[2]);

// BUILTINS
// built_in.c
int			check_built_in(t_command *cmds);
void	choose_builtin(t_shell *shell, t_command *cmd);

// void		execute_built_in(t_shell *shell);
int		execute_built_in(t_shell *shell, t_command *cmd);

// build_in_utils.c
void		print_cd_error(char *dir, t_shell *shell);
int			validate_cd_args(t_shell *shell);
void		sort_env(char **env, int size);
// cd.c
void		ft_cd(t_shell *shell);
// echo.c
// void		ft_echo(t_shell *shell);
void		ft_echo(t_shell *shell, t_command *cmd);
// env.c
void		ft_env(t_shell *shell);
// export.c
void		ft_export(t_shell *shell);
// pwd.c
void		ft_pwd(t_shell *shell);
// unset.c
void		ft_unset(t_shell *shell);
// ft_exit.c
void		ft_exit(t_shell *shell);

//set_unset.c
void		set_env_var(t_shell *shell, const char *key, const char *value);
int			is_valid_identifier(const char *key);

//signals.c
void		setup_signals(t_shell *shell);
void		handle_signal(int signum);

// For debugging
void		print_token_list(t_arg *tokens);
void		print_command_list(t_command *cmd);
void		print_redirections(t_redir *redirs, char *type);

void	execution(t_shell *shell);

#endif