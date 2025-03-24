OS := $(shell uname)
  
ifeq ($(OS), Darwin)  # For me to work on my macOS
	RLFLAGS = -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline -lncurses
else
	RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses
endif

NAME = minishell
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
#RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses
LIBFT = libft/libft.a

SRC = srcs/main.c \
		srcs/execution/execute.c \
		srcs/execution/utils.c \
		srcs/execution/init.c \
		srcs/execution/redirection.c \
		srcs/execution/pipe.c \
		srcs/parsing/get_tokens.c \
		srcs/parsing/get_cmd.c \
		srcs/parsing/env_var.c \
		srcs/parsing/env_var_utils.c \
		srcs/parsing/parse_init.c \
		srcs/parsing/free_parse.c \
		srcs/parsing/parse_utils.c \
		srcs/builtins/built_in.c \
		srcs/builtins/cd.c \
		srcs/builtins/set_unset.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/export.c \
		srcs/builtins/ft_exit.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c

OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
OBJ_DIR = obj

all	: $(LIBFT) $(NAME)

$(NAME) : $(OBJ) $(LIBFT)
	@echo "Compiling $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(RLFLAGS) $(LIBFT)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C libft --no-print-directory

clean:
	@echo "Cleaning project files..."
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C libft clean --no-print-directory

fclean: clean
	@echo "Removing $(NAME)..."
	@rm -f $(NAME)
	@$(MAKE) -C libft fclean --no-print-directory

re : fclean all

.PHONY: all clean fclean re