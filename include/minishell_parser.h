/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/26 14:33:25 by tda-roch         ###   ########.fr       */
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

// TODO: remove debug include before eval
# include "minishell_debug.h"

/*
no (POSIX or otherwise) standard on enviroment variable name size limit
larger than 256: hard to use and read.
larger than 2048: arbitrarily high soft limit: avoids truncation
*/
# define ENV_VAR_NAME_MAX 2048

typedef enum e_token_ty
{
	TOKEN_WORD,
	TOKEN_INBUILT,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_VARIABLE,
	TOKEN_DASH_PARAM,
	TOKEN_FILE_PATH,
	TOKEN_NUMBER,
	TOKEN_BACKSLASH,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_EQUAL,
	UNCLOSED_DOUBLE_QUOTE,
	UNCLOSED_SINGLE_QUOTE,
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
	char	*newline;
	char	*value;
	int		i;
	int		line_len;
	int		var_total;
	int		var_i;
	int		var_name_i;
	int		envp_var_i;
	int		exp_i;
	int		skipped_chars;
	bool	single_quote;
	bool	double_quote;
	bool	var_lookup;
	char	**var_names;
	char	**var_values;
}	t_var_expand;

typedef struct s_msh
{
	t_ast		*ast;
	t_token		*tokens;
	char		**envp;
	char		**export_vars;
	char		**path_dirs;
	char		*line;
	int			err;
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
char		**token_words_to_argv(t_token *tokens, int start, int end);
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
int			count_tokens(char *str);
const char	*get_token_name(t_token_ty type);
const char	*get_token_name_continued(t_token_ty type);
char		*make_word(char *str, int *i, int *err);
int			skip_spaces(int *i, char *str);

// parser/line_var_expand_catch.c
int			get_var_count(char *line);
int			catch_all_vars(t_msh *sh, t_var_expand *ve, char *line);

// parser/line_var_expand.c
int			expand_line(t_msh *sh);

// parser/line_var_expand_helper.c
bool		handle_single_quote(char *line, bool *single_quote, int i);
bool		handle_quotes_for_expansion(char *line, bool *single_quote, bool *double_quote, int i);
int			init_var_expand_arrays(t_msh *sh, t_var_expand *ve);
int			allocate_new_line(t_msh *sh, t_var_expand *ve);
void		reset_var_lookup(t_var_expand *ve);
void		replace_line_and_cleanup(t_msh *sh, t_var_expand *ve);

// parser/tokenize.c
const char	*get_token_name(t_token_ty type);
int			is_builtin(char *str);
t_token		*tokenize(char *input, int *token_count, int *err);
void		free_tokens(t_token **tokens, int amount);

// parser/tokenise.c
int			is_file_path(char *str, int *y);
int			search_for_singlequote(char *str);
void		tokenise_quotes(char *str, t_token *output);
void		tokenise_redirs(char *str, t_token *output);
void		free_tokens(t_token **tokens, int amount);

// utils/parser_isminioperator.c
int			isminioperator(char *token, int i);

// utils/parser_line.c
bool		piped_line(char *line);

// errors
void		int_closed(char *str, int i, char quote);
int			ft_strcmp(const char *s1, const char *s2);
void		*ft_malloc(size_t amount, size_t size);
int			callo_x(void **ptr, size_t nmemb, size_t size);
int			mallo_x(void **ptr, size_t nmemb, size_t size);
int			is_closed(char *str, int i, char quote);
int			unclosed_token(char *str, char token);
int			validate_pipe_syntax(t_token *tokens, int start, int end);

#endif