NAME := minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.

SRCS = functions/ft_atoi.c functions/ft_calloc.c functions/ft_strncmp.c \
	functions/ft_isalnum.c functions/ft_isalpha.c functions/ft_isascii.c functions/ft_isdigit.c \
	functions/ft_isprint.c functions/ft_strlen.c functions/ft_isspace.c \
	functions/ft_isminioperator.c functions/ft_memcpy.c parse.c tokenize.c main.c


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
