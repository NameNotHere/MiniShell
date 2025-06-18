NAME = parser

# compiler settings
CC = cc
CFLAGS = -Wall -Wextra -Werror

# LIBFT settings
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a
LIBS = -Llibft -lft


INCLUDEDIR = include
INCLUDE = -I $(INCLUDEDIR) -I $(LIBFTDIR)
SRCDIR = src
OBJDIR = src/obj

# Source files
SRCS = parser/AST.c \
	parser/main.c \
	parser/lex.c \
	parser/tokenize.c

OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

RM = rm -f

# Default rule
all:
	@$(MAKE) $(NAME)

# Link object files into executable
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME) $(OBJS) $(LIBS)

# compile rules (.c to .o)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re