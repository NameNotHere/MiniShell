# static library name
NAME = libft.a
# compiler for c sources
CC = cc
# compiler for static library
LIBC = ar rcs
# list sources. wildcards are not allowed
SRCS = ft_atoi.c \
		ft_atoll.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isdigit.c \
		ft_isspace.c \
		ft_itoa.c \
		ft_memchr.c \
		ft_memcpy.c \
		ft_realloc.c \
		ft_split.c \
		ft_strchr.c \
		ft_strcmp.c \
		ft_strdup.c \
		ft_strjoin.c \
		ft_strjoin3.c \
		ft_strlen.c \
		ft_strncmp.c \
		ft_strndup.c \
		ft_strnstr.c \

# compiler flags (use ?= to allow parent Makefile to override)
CFLAGS ?= -Wall -Wextra -Werror
# remove files without asking
RM = rm -f
# .o files same name as .c, but in bin folder
DIR_INCLUDE = include
INCLUDE = -I $(DIR_INCLUDE)
SRCDIR = src
OBJDIR = bin
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

# .c to .o compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} ${INCLUDE} -c -o $@ $<

# create static library
$(NAME): $(OBJS)
	${LIBC} ${NAME} ${OBJS}

# default (first) rule
all: ${NAME}

# cleans just the .o files
clean:
	${RM} -r ${OBJDIR}

# cleans .o + the static lib created before
fclean: clean
	${RM} ${NAME} libft.so

# rebuild everything
re: fclean all

so: CFLAGS += -fPIC
so: $(OBJS)
	$(CC) -shared -o libft.so $(OBJS)

.PHONY: all clean fclean re so
