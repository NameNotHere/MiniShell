/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/23 15:24:56 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include "libft.h"

typedef enum e_token_type
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
	TOKEN_LAST
}	t_token_type;

typedef struct s_token
{
	t_token_type	ty;
	char			*word;
}	t_token;

/*
AST DATA STRUCTURE:
*/

/* shell node type, not sure im needing this enum
	removed redir nodes, they enter into cmd only*/
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_UNKNOWN,
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_INPUT,
	REDIR_HEREDOC,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_UNKNOWN
}	t_redir_type;

# define REDIR_INPUT_SYMBOL "<"
# define REDIR_OUTPUT_SYMBOL ">"
# define REDIR_APPEND_SYMBOL ">>"
# define REDIR_HEREDOC_SYMBOL "<<"

/*
* TYPE (t_redir_type)
* STRING → (char *)string = file to be opened or delimiter if heredoc
*/
typedef struct s_redir_node
{
	t_redir_type		type;
	char				*string;
	struct s_redir_node	*next;
}	t_redir_node;


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
typedef struct s_cmd_node
{
	char				*cmd;
	char				**argv;
	bool				built_in;
	t_redir_node		*redir;
}	t_cmd_node;

typedef struct s_ast_node	t_shell_node;

typedef struct s_pipe_node
{
	t_shell_node	*left;
	t_shell_node	*right;
}	t_pipe_node;

/*
AST node for shell commands & pipes,
	- includes a data-named-union of two subtypes: cmd or pipe
*/
typedef struct s_ast_node
{
	t_node_type	nty;
	union
	{
		t_cmd_node			cmd;
		t_pipe_node			pipe;
	};
}	t_ast_node;


// parser/AST.c
void		build_ast(t_ast_node *ast, t_token *tokens);

void		free_ast(t_ast_node *node);

t_ast_node	*make_ast_node(t_node_type type);

t_ast_node	*parse_command_tokens(t_token *tokens, int start, int end);

int			last_token(t_token *tokens);

void		scan_tokens(t_ast_node *ast, t_token *tokens, int start, int end);

// parser/AST.c CONTINUED
// TODO: REMOVE THIS FUNCTION PROTOTYPE BEFORE EVALUATION
// TODO: MAYBE KEEP IN A SEPARATE (NOT SUBMITTED TEST SUITE, FOR DEBUGGING)
void		print_ast(t_ast_node *root);

// parser/lex.c

int			count_tokens(char *str);

int			skip_spaces(int *i, char *str);

char		*make_word(char *str, int *i);

// parser/tokenize.c

const char	*get_token_name(t_token_type type);

int			is_builtin(char *str);

t_token		*tokenize(char *input, int *token_count);

void		free_tokens(t_token *tokens, int amount);

// parser/utils/isminioperator.c

int			isminioperator(char *token, int i);

#endif