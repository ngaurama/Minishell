NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RLFLAGS = -L/usr/lib -I/usr/include -lreadline -lncurses

SRC = srcs/main.c \
		srcs/execute.c \
		srcs/utils.c \
		srcs/add_arg.c \
		srcs/init.c \
		srcs/get_tokens.c \
		srcs/get_cmd.c \
		srcs/parse_init.c

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