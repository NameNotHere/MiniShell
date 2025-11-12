/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/12 09:12:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"
# include "minishell_errors.h"

/*
PRO defaults 0 (false)

IMPORTANT NOTE!

PRO=0 means minishell does not handle any extra features like backslash
escaping variable expansions or characters -- forcing them into literal chars,
unclosed quotes request for additional input, positional variables,
locale syntax, ansi C quoting, etc.

Running the PRO (or PRO=1) version means minishell handles all extra shell
features coded. The extra features can be either non-specifically clearly needed
or requested, but also could be features NOT ALOWED by the subject.pdf
requirements (like handling backslash or interpreting unclosed quotes).
We understand that keeping the PRO 0 by default and requiring the extra optional
define for the compilation, as well as keeping the extra features defined in a
separate header file and code in different folder, and also the case that the
compiler smartly detects unused functions when a define used at compile-time is
different, that minishell is NOT violating the requirements but still allowing
an evaluator that would argue that one of those features are required, to be
able to compile a version handing this or that feature.
*/
# ifndef PRO
#  define PRO 0
# endif

// If PRO mode is enabled, VALIDATE must also be enabled
# if PRO == 1
#  ifdef VALIDATE
#   if VALIDATE == 0
#    undef VALIDATE
#    define VALIDATE 1
#   endif
#  else
#   define VALIDATE 1
#  endif
# endif

/*
VALIDATE mode: controls operator validation (defaults to 1)

VALIDATE=1 (default): validates operators, throws syntax errors for:
- Logical operators: &&, ||, &
- Command separator: ;
These are not supported by this minishell.

VALIDATE=0: disables operator validation, treats all operators as literal text.
Creates an ultra-minimal shell that accepts any input without syntax errors
for unsupported operators.

Usage: make VALIDATE=0
*/
# ifndef VALIDATE
#  define VALIDATE 1
# endif

# define PATH_DEFAULT "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin\
:/sbin:/bin"

/*
no (POSIX or otherwise) standard on enviroment variable name size limit
larger than 256: hard to use and read.
larger than 2048: arbitrarily high soft limit: avoids truncation
*/
# define ENV_VAR_NAME_MAX 2048

/* exit code for syntax errors (bash convention) */
# define EXIT_SYNTAX 2

/* marker for operators in variable expansions before tokenizing */
# define EXP_MARK '\x01'

/* marker for single quotes inside $"..." that should be preserved */
# define ESCAPED_SGL_QUOTE '\x02'

typedef enum e_token_ty
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_DASH_PARAM,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_AMPERSAND,
	TOKEN_SEMICOLON,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LAST
}	t_token_ty;

typedef struct s_readbuf
{
	char	buf[4096];
	ssize_t	len;
	ssize_t	pos;
}	t_readbuf;

/*
	Struct to hold per-call state of the readline_noninteractive.
	- This tracks the accumulating line buffer made and its current length
	during a single call.
	- It is initialized fresh each time and freed/returned when the
	line is emitted. Unlike t_readbuf, this does not persist across calls.
*/
typedef struct s_rln_state
{
	char	*line_made;
	size_t	made_len;
	ssize_t	end;
}	t_rln_state;

typedef struct s_token
{
	t_token_ty	ty;
	char		*word;
}	t_token;

/*
AST DATA STRUCTURE:
*/

/* ast node type enum */
typedef enum e_node_ty
{
	NODE_CMD,
	NODE_PIPE,
	NODE_UNKNOWN,
}	t_node_ty;

/* redirection types enum */
typedef enum e_redir_ty
{
	REDIR_INPUT,
	REDIR_HEREDOC,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_UNKNOWN
}	t_redir_ty;

/* redirection struct */
typedef struct s_redir
{
	t_redir_ty		ty;
	char			*string;
	int				fd;
	bool			quoted;
	struct s_redir	*next;
}	t_redir;

/* command struct */
typedef struct s_cmd
{
	char		*full_cmd;
	char		**argv;
	int			argc;
	bool		built_in;
	bool		not_found;
	bool		permission_denied;
	bool		is_a_dir;
	t_redir		*redir;
}	t_cmd;

/* AST node struct - forward define */
struct	s_ast;

typedef struct s_pipe
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_pipe;

/*
AST node for shell commands & pipes,
	- includes anonymous union of two subtypes: cmd and pipe
*/
typedef struct s_ast
{
	t_node_ty	nty;
	union
	{
		t_cmd	cmd;
		t_pipe	pipe;
	};
}	t_ast;

/* struct for processing variable expansions before tokenizing */
typedef struct s_var_expand
{
	char	var_name_buffer[ENV_VAR_NAME_MAX];
	char	*new_str;
	char	*value;
	int		i;
	int		res_i;
	int		str_len;
	int		var_total;
	int		var_i;
	int		var_name_i;
	int		envp_var_i;
	int		exp_i;
	int		skipped_chars;
	bool	sgl_quote;
	bool	dbl_quote;
	bool	var_lookup;
	bool	is_hdoc;
	char	**var_names;
	char	**var_values;
	int		*skipped;
	int		skip_len;
}	t_var_expand;

/* struct for processing quote removal */
typedef struct s_remove_quotes
{
	int		str_i;
	int		res_i;
	bool	in_sgl_quote;
	bool	in_dbl_quote;
}	t_remove_quotes;

/*minishell data struct */
typedef struct s_msh
{
	t_ast		*ast;
	t_token		*tokens;
	char		**envp;
	char		**export_vars;
	char		**path_dirs;
	char		*line;
	int			exit_code;
	int			saved_exit_code;
	pid_t		last_pid;
	bool		is_interact;
	int			script_fd;
	t_readbuf	readbuf;
}	t_msh;

// parser/ast.c
int			build_ast(t_msh *sh, t_ast *ast, t_token *tokens);
int			scan_tokens(t_msh *sh, t_ast *ast, int start, int end);
int			scan_pipe(t_msh *sh, t_ast *ast, t_token *tokens, int *i);
int			parse_pipe(t_msh *sh, t_ast *ast, int start, int end);

// parser/ast_cmd.c
int			parse_cmd(t_msh *sh, t_ast *ast, int start, int end);
char		**token_words_to_argv(t_token *tokens, int start, int end,
				int argc);
char		*remove_quotes(char *str, int len);

// utils/parser_token.c
bool		is_redir_token(t_token_ty token_type);
bool		is_within_redir_tokens(t_token *tokens, int i);
bool		is_valid_cmd_token(t_token_ty token_type);

// parser/ast_helper.c
void		free_ast(t_ast **node);
void		free_ast_cmd(t_ast *node);
bool		has_pipe(t_ast *ast, t_token *tokens, int start, int end);
int			last_token(t_token *tokens);
t_ast		*make_ast_node(t_node_ty ty);

// parser/ast_redir.c
int			add_redir(t_ast *ast, t_token_ty token_type, char *word);
t_redir_ty	get_redir_type(t_token_ty ty);
int			parse_redir(t_msh *sh, t_ast *ast, int *start, int *end);

// parser/lex.c
int			count_tokens(char *str, int count, int i);
char		*make_token_word(char *str, int *i, int *err);
void		skip_spaces(int *i, char *str);

// expansions/line_var_expand_catch.c
int			get_var_count(char *str, t_var_expand *ve);
int			catch_all_vars(t_msh *sh, t_var_expand *ve, char *str);
bool		is_in_heredoc_delimiter(char *str, int pos);

// expansions/line_var_expand.c
bool		expand_string_variables(t_msh *sh, char **string_ptr, bool is_hdoc);
bool		must_skip_exp(t_var_expand *ve, int index);

// expansions/line_var_expand_exec.c
int			expand_vars(t_var_expand *ve, char *str);

// expansions/line_var_expand_helper_pro.c
bool		advanced_substitutions(t_var_expand *ve, char **str_ptr);

// expansions/line_var_expand_catch_lookup.c
int			catch_var(t_msh *sh, t_var_expand *ve);
int			catch_absent_var(t_msh *sh, t_var_expand *ve);
bool		is_positional_var(t_var_expand *ve, char c);
bool		must_expand_tilde(t_var_expand *ve, char *str, int pos);
int			catch_tilde(t_msh *sh, t_var_expand *ve);

// expansions/advanced_expansions.c - Condition checkers
bool		must_fix_escaped_backslash(t_var_expand *ve, char *str);
bool		must_fix_escaped_dollar(t_var_expand *ve, char *str);
bool		must_fix_escaped_quotes(t_var_expand *ve, char *str);
bool		must_fix_unquoted_backslash(t_var_expand *ve, char *str);
bool		must_fix_locale_syntax(t_var_expand *ve, char *str);
bool		must_fix_ansi_c_quoting(t_var_expand *ve, char *str);

// expansions/advanced_expansions.c - Action functions
void		fix_escaped_backslash(t_var_expand *ve, char *result);
void		fix_escaped_dollar(t_var_expand *ve, char *result);
void		fix_quoted_chars(t_var_expand *ve, char *str, char *result);
void		fix_unquoted_backslash(t_var_expand *ve, char *str, char *result);
void		fix_locale_syntax(t_var_expand *ve, char *result, char *str);
void		fix_ansi_c_quoting(t_var_expand *ve, char *result, char *str);

// expansions/line_var_expand_helper.c
bool		is_quote_free(t_var_expand *ve);
bool		must_expand(t_var_expand *ve, char *str, int pos);
bool		handle_sgl_quote(char *str, bool *sgl_quote, int i);
bool		handle_ve_quote(char *str, bool *sgl_quote, bool *dbl_quote, int i);
int			init_var_expand_arrays(t_msh *sh, t_var_expand *ve);
int			allocate_new_str(t_msh *sh, t_var_expand *ve);
void		reset_var_lookup(t_var_expand *ve);

// parser/tokenize.c
t_token		*tokenize(char *input, int *token_count, int *err);
void		free_tokens(t_token **tokens, int amount);

// utils/parser_isminioperator.c
int			is_operator(char *token, int i);
bool		is_operator_char(char c);

// utils/parser_line.c
bool		piped_line(char *line);

// utils/parser_char.c
bool		is_valid_var_char(int c);
bool		is_sgl_quote(int c);
bool		is_dbl_quote(int c);

// utils/utils_free.c
void		safe_free(void **ptr);
void		safe_free_str(char **ptr);
void		safe_free_2d_string(char ***ptr);
bool		make_string_free(char **string);

// utils/math.c
int			min_int(int a, int b);
int			max_int(int a, int b);

// utils/utils_path.c
char		*make_cmd_full_path(const char *dir, const char *cmd);
char		*get_valid_cmd_full_path(char **path_dirs, char *cmd);
char		*get_path_from_env(char **envp);
int			update_path_dirs(char ***path_dirs, char **envp);

// parser/errors.c
void		int_closed(char *str, int i, char quote);
int			ft_strcmp(const char *s1, const char *s2);

/* xe_ functions: with error pointer parameter */
int			xe_malloc(void **ptr, int *err, size_t nmemb, size_t size);
int			xe_calloc(void **ptr, int *err, size_t nmemb, size_t size);

/* Type-specific xe_calloc wrappers */
int			xe_calloc_char(char **ptr, int *err, size_t count);
int			xe_calloc_token(t_token **ptr, int *err, size_t count);
int			xe_calloc_int(int **ptr, int *err, size_t count);
int			xe_calloc_charptr(char ***ptr, int *err, size_t count);

/* x_ functions: without error pointer parameter (uses local error) */
int			x_malloc(void **ptr, size_t nmemb, size_t size);
int			x_calloc(void **ptr, size_t nmemb, size_t size);

/* Type-specific x_calloc wrappers */
int			x_calloc_char(char **ptr, size_t count);
int			x_calloc_token(t_token **ptr, size_t count);
int			x_calloc_int(int **ptr, size_t count);
int			x_calloc_charptr(char ***ptr, size_t count);
int			x_calloc_redir(t_redir **ptr, size_t count);
int			x_calloc_ast(t_ast **ptr, size_t count);

// parser/parser_validation.c
int			validate_pipe_syntax(t_token *tokens, int start, int end);

// utils/parser_detect_unsupported_operator.c
int			detect_unsupported_operator(t_token *tokens);
int			process_unsupported_operator_error(t_msh *sh);

// utils/utils_error.c
void		msg_err(const char *error);
void		msg_err_2(const char *str1, const char *str2);
void		msg_err_3(const char *str1, const char *str2, const char *str3);

// utils/shortcuts/utils_r_err_msg.c
int			r_msg_err_free_str(const char *error, char **to_free, int ret);
void		*msg_err_null(const char *error);
void		*r_free_str_null(char **to_free);

// utils/shortcuts/utils_r_plus.c
int			r_free_str(char **to_free, int ret);
int			r_free_two_str(char **str_a, char **str_b, int ret);
void		*r_free_null(void **ptr);

// utils/utils_error2.c
int			change_dir_or_error(t_msh *sh, char **directory);
void		msg_perr(const char *error);

#endif
