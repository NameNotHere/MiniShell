

# USING WILDCARDS NOT SURE THATS ALLOXED
NAME    := minishell
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror
SRCS    := $(wildcard *.c)
OBJS    := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
