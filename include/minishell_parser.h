/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/19 04:15:25 by tda-roch         ###   ########.fr       */
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
	TOKEN_PARAM,
	TOKEN_FILE_PATH,
	TOKEN_NUMBER,
	TOKEN_BACKSLASH,
	TOKEN_AND,
	TOKEN_OR
}	t_token_type;

typedef struct s_token
{
	t_token_type	ty;
	char			*word;
}	t_token;

/*
AST DATA STRUCTURE:
*/

/* shell node type*/
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_HEREDOC
}	t_node_type;

/* out modes*/
typedef enum e_out_mode
{
	CREATE_TRUNCATE,
	APPEND,
}	t_out_mode;

/*attaches only in: s_redir_in_node*/
typedef struct s_heredoc_node
{
	char	*delimiter;
}	t_heredoc_node;

/*
* IN → (int)fd or (char *)filepath to be opened (to determine later
	- but probably the latter)
* HEREDOC NODE → DEFAULTS TO  NULL
*/
typedef struct s_redir_in_node
{
	char			*in;
	t_heredoc_node	heredoc;
}	t_redir_in_node;

/*
* OUT → (int)fd or (char *)filepath to be opened (to determine later
	- but probably the latter) → DEFAULTS TO STDOUT
* OUT_MODE → (create/truncate OR append ) ->  DEFAULTS TO create/truncate
*/
typedef struct s_redir_out_node
{
	char	*out;
	int		out_mode;
}	t_redir_out_node;

/*
* (char *) path → DEFAULTS TO current directory
* (char *) cmd_name (file name (stripped of path) or builtin name)
* (bool) built-in → defaults to false
* (char **)argv → each  argument in a separate string, NULL terminated.
		→ DEFAULTS to NULL
* REDIR IN NODE → DEFAULTS TO STDIN
* REDIR OUT NODE → DEFAULTS TO STDOUT, (create/truncate)
*/
typedef struct s_cmd_node
{
	char				*path;
	char				*cmd_name;
	char				**argv;
	bool				built_in;
	t_redir_in_node		*redir_in;
	t_redir_out_node	*redir_out;
}	t_cmd_node;

typedef struct s_shell_node	t_shell_node;

typedef struct s_pipe_node
{
	t_shell_node	*left;
	t_shell_node	*right;
}	t_pipe_node;

/* shell AST node */
typedef struct s_shell_node
{
	t_node_type	nty;
	union u_node_data
	{
		t_cmd_node			cmd;
		t_pipe_node			pipe;
		t_heredoc_node		heredoc;
		t_redir_in_node		redir_in;
		t_redir_out_node	redir_out;
	}	data;
}	t_shell_node;


int			count_tokens(char *str);

t_token		token(char *str);

t_token		*tokenize(char *input, int *token_count);

int			parse_tokens(t_token *list);

int			skip_spaces(int *i, char *str);

int			is_builtin(char *str);

int			isminioperator(char *token, int i);

char		*make_word(char *str, int *i);

const char	*get_token_name(t_token_type type);

#endif