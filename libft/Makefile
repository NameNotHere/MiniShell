CC = cc
CFLAGS ?= -Wall -Wextra -Werror -fsanitize=address,undefined,leak -g3 -fno-omit-frame-pointer
LDFLAGS ?=
SRC = ft_atoi.c ft_atoll.c ft_calloc.c ft_memcpy.c ft_strchr.c ft_bzero.c ft_isalnum.c \
		ft_isalpha.c ft_isdigit.c ft_strlen.c ft_strdup.c ft_strncmp.c ft_strnstr.c \
		ft_strjoin.c ft_split.c ft_itoa.c ft_memchr.c \
		ft_isspace.c ft_strjoin3.c \
		ft_strcmp.c ft_strndup.c ft_realloc.c

BONUS =

OBJ = $(SRC:.c=.o)
BONUSOBJ = $(BONUS:.c=.o)

NAME = libft.a

all: $(NAME)

$(NAME): $(OBJ)
	ar -rcs $@ $(OBJ)

bonus: $(NAME) $(BONUSOBJ)
	ar -rcs $(NAME) $(BONUSOBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUSOBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
