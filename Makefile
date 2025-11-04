NAME = minishell

#TODO: LAST remove -g before submitting
# compiler settings
CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=address,undefined,leak -g3 -fno-omit-frame-pointer
CFLAGS_DEBUG = -Wall -Werror -Wextra -g3 -fno-omit-frame-pointer
LDFLAGS = -lreadline -Llibft -lft

# PRO mode: enable extra features (positional params, etc)
ifdef PRO
CFLAGS += -DPRO=$(PRO)
CFLAGS_DEBUG += -DPRO=$(PRO)
endif

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

SRCS = 	signals/signals.c \
	signals/signals_execution.c \
	signals/signals_interactive.c \
	signals/signals_heredoc.c \
	minishell_initialize.c \
	minishell_main.c \
	builtins/builtins.c \
	builtins/ft_export.c \
	builtins/builtins_echo.c \
	builtins/builtins_exit_export.c \
	exec/lookup_cmd_fullpath.c \
	exec/execute.c \
	exec/execute_cleanup.c \
	exec/execute_cmd.c \
	exec/execute_cmd_single.c \
	exec/execute_cmd_redir.c \
	exec/execute_cmd_redir_open.c \
	exec/heredoc.c \
	exec/heredoc_assist.c \
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
	parser/parse_line.c \
	parser/parse_validation.c \
	parser/tokenise_types.c \
	parser/tokenize.c \
	parser/is_escaped.c \
	utils/envp_assistance_array.c \
	utils/detect_logical_op.c \
	utils/ft_strndup.c \
	utils/ft_strcmp.c \
	utils/ft_realloc.c \
	utils/has_quotes.c \
	utils/parser_is_operator.c \
	utils/parser_line.c \
	utils/unclosed_quotes.c \
	utils/utils_char.c \
	utils/utils_dup2.c \
	utils/utils_env.c \
	utils/utils_error.c \
	utils/utils_error_2.c \
	utils/utils_exit.c \
	utils/utils_fd.c \
	utils/utils_free.c \
	utils/utils_math.c \
	utils/utils_path.c \
	utils/utils_readline.c \
	utils/utils_readline_state.c \
	utils/utils_ret_err_msg.c \
	utils/utils_ret_plus.c \
	utils/utils_set_exit_code.c \
	utils/utils_string.c \
	utils/utils_string_array.c \
	utils/utils_token.c \
	debug/utils_debug.c

OBJS = $(SRCS:.c=.o)
OBJS := $(addprefix $(OBJDIR)/, $(OBJS))

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(NAME) $(OBJS) $(LDFLAGS)

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
	$(RM) -r $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

bonus: all

debug: fclean
	$(MAKE) CFLAGS="$(CFLAGS_DEBUG)" all
	@echo "debug build made"

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --trace-children=yes --suppressions=rl.supp ./$(NAME)

.PHONY: all clean fclean re bonus debug