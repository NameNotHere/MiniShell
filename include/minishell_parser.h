/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/07/05 11:55:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include <errno.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <limits.h>
# include "libft.h"

/*
no (POSIX or otherwise) standard on enviroment variable name size limit
larger than 256: hard to use and read.
larger than 2048: arbitrarily high soft limit: avoids truncation
*/
# define ENV_VAR_NAME_MAX 2048

# define REDIR_INPUT_PRINT "< INPUT"
# define REDIR_OUTPUT_PRINT "> OUTPUT"
# define REDIR_APPEND_PRINT ">> APPEND"
# define REDIR_HEREDOC_PRINT "<< HEREDOC"

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

typedef struct s_token
{
	t_token_ty	ty;
	char		*word;
}	t_token;

/*
AST DATA STRUCTURE:
*/

/* shell node type, not sure im needing this enum
	removed redir nodes, they enter into cmd only*/
typedef enum e_node_ty
{
	NODE_CMD,
	NODE_PIPE,
	NODE_UNKNOWN,
}	t_node_ty;

typedef enum e_redir_ty
{
	REDIR_INPUT,
	REDIR_HEREDOC,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_UNKNOWN
}	t_redir_ty;

/*
* TYPE (t_redir_type)
* STRING → (char *)string = file to be opened or delimiter if heredoc
*/
typedef struct s_redir
{
	t_redir_ty		ty;
	char			*string;
	struct s_redir	*next;
}	t_redir;


/*
* 	(REMOVED PATH)
* (char *) cmd (cmd name or cmd file name with or without path: absolute,
	relative, etc
		TODO: check if I can get rid of cmd, use argv[0] instead.
* (bool) built-in → defaults to false
	TODO: also check if we will need this
* (char **)argv → each  argument in a separate string, NULL terminated.
		→ DEFAULTS to NULL
* REDIR → DEFAULTS TO STDIN & STDOUT -> (basic linked list)
*/
typedef struct s_cmd
{
	char		*full_cmd;
	char		**argv;
	bool		built_in;
	bool		not_found;
	t_redir		*redir;
}	t_cmd;

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
	bool	var_lookup;
	char	**var_names;
	char	**var_values;
}	t_var_expand;

/*
TODO: remove comments
Removed from the pipex struct:
typedef struct s_pipex
{
	int		argc; -> REMOVED -> not needed unless we have options or scripts
	char	**argv; -> REMOVED -> not needed unless we have options or scripts
	char	**envp;
	char	***cmd_arg; -> REMOVED -> ast cmd nodes have them
	bool	*cmd_not_found; -> REMOVED -> ast cmd nodes SHOULD have them
	char	**cmd_path; -> REMOVED -> ast cmd nodes should have them
	char	**path_dirs;
	size_t	cmd_offset; -> REMOVED (NOT NEEDED) -> this is for pipex logic
	size_t	cmd_total; -> REMOVED (NOT NEEDED) -> this is for pipex logic
	int		fdin; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		fdout; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		outfile_flags; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	bool	hdoc; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	char	*infile; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	char	*outfile; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		exit_code;
}	t_pipex;
*/
typedef struct s_msh
{
	t_ast	*ast;
	t_token	*tokens;
	char	**envp;
	char	**path_dirs;
	char	*line;
	int		err;
	int		exit_code;
}	t_msh;

// parser/ast.c
int			build_ast(t_msh *sh, t_ast *ast, t_token *tokens);

int			scan_tokens(t_msh *sh, t_ast *ast, int start, int end);

void		scan_pipe(t_msh *sh, t_ast *ast, t_token *tokens, int *i);

void		parse_pipe(t_msh *sh, t_ast *ast, int start, int end);

// parser/ast_cmd.c

void		parse_cmd(t_msh *sh, t_ast *ast, int start, int end);

char		**token_words_to_argv(t_token *tokens, int start, int end);

// parser/ast_helper.c

void		free_ast(t_ast **node);

void		free_ast_cmd(t_ast *node);

bool		has_pipe(t_token *tokens, int start, int end);

int			last_token(t_token *tokens);

t_ast		*make_ast_node(t_node_ty ty);

// TODO: REMOVE ALL FUNCS AND FILES FOR AST PRINT BEFORE EVAL
// parser/ast_print.c
char		*get_redir_symbol(t_redir_ty ty);

void		print_ast(t_ast *root);

void		print_ast_cmd(t_ast *node);

void		print_ast_node(t_ast *node, int depth);

int			print_build_ast(t_msh *sh, t_ast *ast, t_token *tokens);

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

int			init_var_expand_arrays(t_msh *sh, t_var_expand *ve);

int			allocate_new_line(t_msh *sh, t_var_expand *ve);

void		reset_var_lookup(t_var_expand *ve);

void		replace_line_and_cleanup(t_msh *sh, t_var_expand *ve);

// parser/tokenize.c

const char	*get_token_name(t_token_ty type);

int			is_builtin(char *str);

t_token		*tokenize(char *input, int *token_count, int *err);

void		free_tokens(t_token **tokens, int amount);

// utils/parser_isminioperator.c

int			isminioperator(char *token, int i);

// utils/parser_line.c

bool		piped_line(char *line);

// errors
void		int_closed(char *str, int i, char quote);

void		*ft_malloc(size_t amount, size_t size);

int			callo_x(void **ptr, size_t nmemb, size_t size);

int			mallo_x(void **ptr, size_t nmemb, size_t size);

int			is_closed(char *str, int i, char quote);

int			unclosed_token(char *str, char token);

#endif