# THIS MAKEFILE COMPILES 2 separate executables:
# parser and minishell
# TODO: before eval, this makefile will only compile minishell (cleanup b4 eval)
# TODO: remove all debug folder references in sources.

# executables
NAME = minishell
PARSER = parser

#TODO: remove -g before submitting
# compiler settings
CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address,undefined,leak -g3 -fno-omit-frame-pointer
CFLAGS_VALGRIND = -Wall -Werror -Wextra -g3 -fno-omit-frame-pointer
LDFLAGS = -lreadline -Llibft -lft

# LIBFT settings
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

# DIR settings
INCLUDEDIR = include
INCLUDE = -I $(INCLUDEDIR) -I $(LIBFTDIR)
SRCDIR = src/
OBJDIR = bin

# Default rule
all:
	@$(MAKE) $(NAME)

# ***** MINISHELL SRCS *****
SRCS = 	signals/signals.c \
	signals/signals_execution.c \
	signals/signals_interactive.c \
	signals/signals_heredoc.c \
	minishell_main.c \
	minishell_line.c \
	builtins/builtins.c \
	builtins/builtins_echo.c \
	builtins/builtins_exit_export.c \
	exec/lookup_cmd_fullpath.c \
	exec/execute.c \
	exec/execute_cleanup.c \
	exec/execute_cmd.c \
	exec/execute_cmd_redir.c \
	exec/execute_cmd_redir_open.c \
	exec/heredoc.c \
	exec/safe_fork.c \
	exec/safe_pipe.c \
	parser/ast.c \
	parser/ast_cmd.c \
	parser/ast_helper.c \
	parser/ast_redir.c \
	parser/errors.c \
	parser/lex.c \
	parser/is_builtin.c \
	parser/line_var_expand.c \
	parser/line_var_expand_catch.c \
	parser/line_var_expand_helper.c \
	parser/tokenise_types.c \
	parser/tokenize.c \
	utils/parser_isminioperator.c \
	utils/parser_line.c \
	utils/utils_char.c \
	utils/utils_dup2.c \
	utils/utils_error.c \
	utils/utils_exit.c \
	utils/utils_fd.c \
	utils/utils_env.c \
	utils/ft_strndup.c \
	utils/utils_free.c \
	utils/ft_realloc.c \
	utils/utils_path.c \
	utils/utils_readline.c \
	utils/utils_readline_state.c \
	utils/utils_string.c \
	utils/utils_string_array.c \
	utils/envp_assistance_array.c \
	debug/ast_print.c \
	debug/process_debug.c \
	debug/utils_debug.c
OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME) $(OBJS) $(LDFLAGS)

# ***** PARSER SRCS *****
PARSER_SRCS = parser/ast.c \
	parser/ast_cmd.c \
	parser/ast_helper.c \
	parser/ast_redir.c \
	parser/errors.c \
	parser/lex.c \
	parser/tokenise_types.c \
	parser/tokenize.c \
	utils/parser_isminioperator.c \
	utils/utils_free.c \
	debug/utils_debug.c \
	debug/ast_print.c

PARSER_OBJS = $(PARSER_SRCS:.c=.o)
PARSER_OBJS := $(addprefix $(OBJDIR)/, $(PARSER_OBJS))

$(PARSER): $(LIBFT) $(PARSER_OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(PARSER) $(PARSER_OBJS) $(LDFLAGS)

# shell commands
RM = rm -f

# compile rules (.c to .o)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

# libft maker
$(LIBFT):
	@$(MAKE) -C $(LIBFTDIR)

clean:
	$(RM) $(OBJS)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	$(RM) $(NAME)
	$(RM) $(PARSER)
	$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

bonus: all

valgrind: fclean
	$(MAKE) CFLAGS="$(CFLAGS_VALGRIND)" all
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --suppressions=minishell.supp ./$(NAME)

.PHONY: all clean fclean re bonus