OS := $(shell uname)
  
ifeq ($(OS), Darwin)  # For me to work on my macOS
    RLFLAGS = -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline
else
	RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses
endif

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
# RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses

SRC = srcs/main.c \
		srcs/execute.c \
		srcs/utils.c \
		srcs/add_arg.c \
		srcs/init.c \
		srcs/get_tokens.c \
		srcs/get_cmd.c \
		srcs/parse_init.c
		srcs/builtins/built_in.c \
		srcs/builtins/cd.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/builtins/export.c \
		srcs/builtins/pwd.c \
		srcs/builtins/unset.c

OBJ = $(SRC:.c=.o)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(RLFLAGS)

all	: $(NAME)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re

