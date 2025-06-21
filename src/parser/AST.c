/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/20 19:34:47 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell_parser.h"

/*
AST DATASTRUCTURE ADDED TO minishell.h
*/


/*
CMD only tokens sent here (knowing start and end of cmd tokens):
	- 1st - will extract command name (with or without path)
	- process arguments (expand variables, remove quotes)
	- detect redirections (extract all redir tokens from CMD parsing and use
	them to add redir_in_node or redir_out_node)
	- validate most things (syntax errors, invalid built in params)
	- NOT validate things that are supposed to fail in execve (ex: invalid path)
	- return clean AST node
*/
void	parse_cmd(t_ast_node *ast, t_token *tokens, int start, int end)
{
	(void)tokens;
	(void)ast;
	printf("parsing command tokens now\n" \
		"start token: %d, end token: %d\n",
		start,
		end);
	// PARSING CMD HERE -->
	// 1. validade syntax,
	// 2. validate options,
	// 3. extract and process redir tokens into redir nodes
	// 4. expand vars,
	// 5. cleanup then build argv.
	// etc.
	return ;
}

void	parse_pipe(t_ast_node *ast, t_token *tokens, int start, int end)
{
	if (tokens && tokens[0].word)
		printf("parse pipe:\n");
	else
		return ;
		// return (NULL);
	printf("parsing a pipe\n" \
		"start token: %d, end token: %d\n",
		start,
		end);
	if (ast == NULL)
		printf("ast not initialized yet, maloc it here?\n");
	printf("add a pipe node in the right place (root or right)\n"
		"next, add the command on left node (send to parse_cmd with end-1)");
	// parse_cmd() --> add correctly
	return ;
}

/*scanning if pipe is found, if yes, call parsing with start/end */
void	scan_pipe(t_ast_node *ast, t_token *tokens, int *i)
{
	int	start;

	start = *i;
	if (tokens[start].word == NULL)
		return ;
	while (tokens[*i].word)
	{
		if (tokens[*i].ty == TOKEN_PIPE)
		{
			parse_pipe(ast, tokens, start, *i);
			return ;
		}
		(*i)++;
	}
	(*i)--;
	parse_cmd(ast, tokens, start, *i);
	return ;
}

/*
creates the ast node pointer then starts scan
keeps scanning while there are tokens in line
--> using iterative instead of recursive approach - safer? probably
*/
void	build_ast(t_ast_node *ast, t_token *tokens)
{
	int			i;

	if (tokens == NULL || tokens[0].word == NULL)
		// return (NULL);
		return ;
	i = -1;
	while (tokens[++i].word)
		scan_pipe(ast, tokens, &i);
	return ;
}
