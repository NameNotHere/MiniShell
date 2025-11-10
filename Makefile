NAME = minishell

# compiler settings
CC = cc
CFLAGS = -Wall -Werror -Wextra
CFLAGS += -g3 -O0
LDFLAGS = -lreadline -Llibft -lft

# PRO mode: enable extra features (positional params, etc)
ifdef PRO
CFLAGS += -DPRO=$(PRO)
endif

# VALIDATE mode: control operator validation (&&, ||, &, ;)
ifdef VALIDATE
CFLAGS += -DVALIDATE=$(VALIDATE)
endif

########################################################################
# TODO: remove below for evaluation                                    #
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS += -fsanitize=address,undefined
# TODO: remove above for evaluation                                    #
########################################################################

CFLAGS_DEBUG = $(CFLAGS)
CFLAGS_DEBUG += -fsanitize=address,undefined -fno-omit-frame-pointer
########################################################################
# CFLAGS_DEBUG CHOICE                                                  #
CFLAGS_DEBUG += -Og
# OR (substitutes: for checking uninitialized, it needs -O1)           #
# CFLAGS_DEBUG += -O1 -Wuninitialized                                  #
########################################################################
LDFLAGS_DEBUG = $(LDFLAGS)
LDFLAGS_DEBUG += -fsanitize=address,undefined

CFLAGS_OPTIMAL = -Wall -Werror -Wextra -O3 -flto
LDFLAGS_OPTIMAL = $(LDFLAGS)
LDFLAGS_OPTIMAL += -flto

# UBSan-only build (for focused undefined behavior testing)
CFLAGS_UBSAN = -Wall -Werror -Wextra -g3 -O1
CFLAGS_UBSAN += -fsanitize=undefined -fno-omit-frame-pointer
CFLAGS_UBSAN += -fno-sanitize-recover=all  # abort on first UB
LDFLAGS_UBSAN = $(LDFLAGS)
LDFLAGS_UBSAN += -fsanitize=undefined

# Memory Sanitizer (detects uninitialized memory reads)
CFLAGS_MSAN = -Wall -Werror -Wextra -g3 -O1
CFLAGS_MSAN += -fsanitize=memory -fno-omit-frame-pointer
CFLAGS_MSAN += -fsanitize-memory-track-origins=2
LDFLAGS_MSAN = -lreadline -Llibft -lft  # Base LDFLAGS without ASan
LDFLAGS_MSAN += -fsanitize=memory

# FORTIFY_SOURCE build (buffer overflow detection)
CFLAGS_FORTIFY = -Wall -Werror -Wextra -g3 -O2
CFLAGS_FORTIFY += -D_FORTIFY_SOURCE=2
CFLAGS_FORTIFY += -fstack-protector-strong
LDFLAGS_FORTIFY = $(LDFLAGS)

# LIBFT settings
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a

# DIR settings
INCLUDEDIR = include
INCLUDE = -I $(INCLUDEDIR) -I $(LIBFTDIR)/include
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
	parser/fix_slash_set_skip_helper.c \
	parser/line_var_expand.c \
	parser/line_var_expand_catch.c \
	parser/line_var_expand_helper.c \
	parser/parse_line.c \
	parser/parse_validation.c \
	parser/tokenize.c \
	parser/is_escaped.c \
	utils/envp_assistance_array.c \
	utils/detect_unsupported_operator.c \
	utils/has_quotes.c \
	utils/parser_is_operator.c \
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
	utils/utils_r_err_msg.c \
	utils/utils_r_plus.c \
	utils/utils_r_set_exit.c \
	utils/utils_set_exit_code.c \
	utils/utils_string.c \
	utils/utils_string_array.c \
	utils/utils_token.c \

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
	@$(MAKE) -C $(LIBFTDIR) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"

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
	$(MAKE) CFLAGS="$(CFLAGS_DEBUG)" LDFLAGS="$(LDFLAGS_DEBUG)" all
	@echo "debug build made"

optimal: fclean
	$(MAKE) CFLAGS="$(CFLAGS_OPTIMAL)" LDFLAGS="$(LDFLAGS_OPTIMAL)" all
	@echo "optimal build made"

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --trace-children=yes --suppressions=rl.supp ./$(NAME)

ubsan: fclean
	$(MAKE) CFLAGS="$(CFLAGS_UBSAN)" LDFLAGS="$(LDFLAGS_UBSAN)" all
	@echo "UBSan build made - detects undefined behavior"

msan: fclean
	@echo "Building with MSan (including libft)..."
	$(MAKE) CFLAGS="$(CFLAGS_MSAN)" LDFLAGS="$(LDFLAGS_MSAN)" all
	@echo "MSan build made - detects uninitialized memory reads"

fortify: fclean
	$(MAKE) CFLAGS="$(CFLAGS_FORTIFY)" LDFLAGS="$(LDFLAGS_FORTIFY)" all
	@echo "FORTIFY_SOURCE build made - detects buffer overflows"

.PHONY: all clean fclean re bonus debug optimal ubsan msan fortify
