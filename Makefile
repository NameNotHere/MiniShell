# THIS MAKEFILE COMPILES 3 separate executables:
# parser, pipex (interactive) and minishell
# TODO: minishell not implemented yet, parser is the minishell parser
# TODO: before eval, this makefile will only compile minishell (cleanup b4 eval)
# TODO: remove all debug folder references in sources.

# executables
NAME = minishell
# PIPEX = pipex
PARSER = parser

#TODO: remove -g before submitting
# compiler settings
CC = cc
CFLAGS = -Wall -Werror -Wextra -g
# LDFLAGS = -lreadline
LDFLAGS = -lreadline -Llibft -lft

# LIBFT settings
LIBFTDIR = libft
LIBFT = $(LIBFTDIR)/libft.a
# LIBS = -Llibft -lft

# DIR settings
INCLUDEDIR = include
INCLUDE = -I $(INCLUDEDIR) -I $(LIBFTDIR)
SRCDIR = src/
OBJDIR = src/obj

# Default rule
all:
	@$(MAKE) $(NAME)
	@$(MAKE) $(PARSER)
# 	@$(MAKE) $(PIPEX)

# ***** MINISHELL SRCS *****
SRCS = minishell_main.c \
	minishell_line.c \
	exec/filenavs.c \
	exec/lookup_cmd_fullpath.c \
	exec/execute.c \
	exec/execute_cmd.c \
	exec/execute_cmd_redir.c \
	exec/execute_cmd_redir_open.c \
	parser/ast.c \
	parser/ast_cmd.c \
	parser/ast_helper.c \
	parser/ast_redir.c \
	parser/errors.c \
	parser/lex.c \
	parser/line_var_expand.c \
	parser/line_var_expand_catch.c \
	parser/line_var_expand_helper.c \
	parser/tokenize.c \
	utils/parser_isminioperator.c \
	utils/parser_line.c \
	utils/utils_char.c \
	utils/utils_dup2.c \
	utils/utils_error.c \
	utils/utils_exit.c \
	utils/utils_fd.c \
	utils/utils_env.c \
	utils/utils_free.c \
	utils/utils_path.c \
	utils/utils_readline.c \
	utils/utils_string.c \
	utils/utils_string_array.c \
	utils/envp_assistance_array.c \
	signals/signals.c \
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
	parser/main.c \
	parser/tokenize.c \
	utils/parser_isminioperator.c \
	debug/utils_debug.c \
	debug/ast_print.c
PARSER_OBJS = $(PARSER_SRCS:.c=.o)
PARSER_OBJS := $(addprefix $(OBJDIR)/, $(PARSER_OBJS))

$(PARSER): $(LIBFT) $(PARSER_OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(PARSER) $(PARSER_OBJS) $(LDFLAGS)

# ***** PIPEX SRCS *****
# PIPEX_SRCS = pipex/pipex.c \
# 		pipex/pipex_main.c \
# 		pipex/pipex_process.c \
# 		pipex/pipex_heredoc.c \
# 		pipex/pipex_initialize.c \
# 		pipex/pipex_interactive.c \
# 		pipex/utils_error.c \
# 		pipex/utils_exit.c \
# 		pipex/utils_free.c \
# 		pipex/utils_mem.c \
# 		pipex/utils_path.c \
# 		pipex/utils_split_quotes.c \
# 		pipex/utils_split_single_delimiter.c \
# 		pipex/utils_string.c

# PIPEX_OBJS = $(PIPEX_SRCS:.c=.o)
# PIPEX_OBJS := $(addprefix $(OBJDIR)/, $(PIPEX_OBJS))

# $(PIPEX): $(LIBFT) $(PIPEX_OBJS)
# 	$(CC) $(CFLAGS) $(INCLUDE) -o $(PIPEX) $(PIPEX_OBJS) $(LDFLAGS)

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
# 	$(RM) $(PIPEX)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus