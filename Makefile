# OS := $(shell uname)
  
# ifeq ($(OS), Darwin)  # For me to work on my macOS
#     RLFLAGS = -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline
# else
# 	RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses -ggdb3
# endif

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses -ggdb3
LIBFT = libft/libft.a

SRC = srcs/main.c \
		srcs/execute.c \
		srcs/utils.c \
		srcs/add_arg.c \
		srcs/init.c \
		srcs/get_tokens.c \
		srcs/get_cmd.c \
		srcs/parse_init.c \
		srcs/builtins/built_in.c \
		srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/export.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c \
		srcs/builtins/expand_variable.c

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

