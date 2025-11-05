/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/05 01:28:18 by tda-roch         ###   ########.fr       */
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
PRO defaults 0
PRO=0 means minishell does not handle any extra features (backslashes, unclosed
quotes etc)
PRO=1 means minishell handles all extra features coded
*/
# ifndef PRO
#  define PRO 0
# endif

// TODO: LAST remove debug include before eval
# include "minishell_debug.h"

# define PATH_DEFAULT "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin\
:/sbin:/bin"

/*
no (POSIX or otherwise) standard on enviroment variable name size limit
larger than 256: hard to use and read.
larger than 2048: arbitrarily high soft limit: avoids truncation
*/
# define ENV_VAR_NAME_MAX 2048

/* marker for operators in variable expansions before tokenizing */
# define EXP_MARK '\x01'

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

typedef struct s_envp
{
	char			*name;
	char			*value;
	struct s_envp	*next;
}	t_envp;

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
void		scan_pipe(t_msh *sh, t_ast *ast, t_token *tokens, int *i);
void		parse_pipe(t_msh *sh, t_ast *ast, int start, int end);

// parser/ast_cmd.c
void		parse_cmd(t_msh *sh, t_ast *ast, int start, int end);
char		**token_words_to_argv(t_token *tokens, int start, int end,
				int argc);
char		*remove_quotes(char *str, int len);

// utils/utils_token.c
bool		is_redir_token(t_token_ty token_type);
bool		is_within_redir_tokens(t_token *tokens, int i);
bool		is_valid_cmd_token(t_token_ty token_type);

// parser/ast_helper.c
void		free_ast(t_ast **node);
void		free_ast_cmd(t_ast *node);
bool		has_pipe(t_token *tokens, int start, int end);
int			last_token(t_token *tokens);
t_ast		*make_ast_node(t_node_ty ty);

// parser/ast_redir.c
void		add_redir(t_ast *ast, t_token_ty token_type, char *word);
t_redir_ty	get_redir_type(t_token_ty ty);
void		parse_redir(t_msh *sh, t_ast *ast, int *start, int *end);

// parser/lex.c
int			count_tokens(char *str, int count, int i);
char		*make_token_word(char *str, int *i, int *err);
void		skip_spaces(int *i, char *str);

// parser/line_var_expand_catch.c
int			get_var_count(char *str, t_var_expand *ve);
int			catch_all_vars(t_msh *sh, t_var_expand *ve, char *str);
bool		is_in_heredoc_delimiter(char *str, int pos);

// parser/line_var_expand.c
bool		expand_string_variables(t_msh *sh, char **string_ptr, bool is_hdoc);
bool		must_skip_exp(t_var_expand *ve, int index);

// parser/line_var_expand_helper.c
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

// utils/utils_char.c
bool		ft_valid_var_char(int c);
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
void		*ft_malloc(size_t amount, size_t size);
int			x_calloc(void **ptr, int *err, size_t nmemb, size_t size);
int			x_malloc(void **ptr, int *err, size_t nmemb, size_t size);
int			x_malloc_char(char **ptr, int *err, size_t count);
int			x_calloc_char(char **ptr, int *err, size_t count);
int			x_malloc_token(t_token **ptr, int *err, size_t count);
int			x_calloc_token(t_token **ptr, int *err, size_t count);
int			x_calloc_int(int **ptr, int *err, size_t count);
int			is_closed(char *str, int i, char quote);
int			unclosed_token(const char *str, char token);

// parser/parser_validation.c
int			validate_pipe_syntax(t_token *tokens, int start, int end);
int			validate_semicolon_syntax(t_token *tokens);

// utils/detect_logical_op.c
int			detect_logical_op_token(t_token *tokens);
int			process_logical_op_syntax_error(t_msh *sh);

// utils/utils_error.c
void		msg_err(const char *error);
void		msg_err_2(const char *str1, const char *str2);
void		msg_err_3(const char *str1, const char *str2, const char *str3);

// utils/utils_ret_err_msg.c
int			ret_msg_free_str(const char *error, char **to_free, int ret);

// utils/utils_ret_plus.c
int			ret_free_str(char **to_free, int ret);
int			ret_free_two_str(char **str_a, char **str_b, int ret);

// utils/utils_error2.c
int			set_dir_or_error(t_msh *sh, char **directory);
void		ms_perror(const char *error);

// utils/has_quotes.c
bool		has_quotes(const char *str);
bool		has_single_quotes(const char *str);

// parser/is_escaped.C
bool		is_escaped(const char *str, int i);

#endif