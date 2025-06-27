/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/27 04:29:23 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
creates the ast node pointer then starts scan
keeps scanning while there are tokens in line
--> using iterative instead of recursive approach - safer? probably
*/
void	build_ast(t_ast *ast, t_token *tokens)
{
	if (tokens == NULL || tokens[0].word == NULL)
		return ;
	scan_tokens(ast, tokens, 0, last_token(tokens));
	return ;
}

void	scan_tokens(t_ast *ast, t_token *tokens, int start, int end)
{
	int		i;
	t_ast	*current_node;

	current_node = ast;
	i = start;
	if (has_pipe(tokens, start, end))
		scan_pipe(current_node, tokens, &i);
	else
		parse_cmd(current_node, tokens, start, end);
}

/*scanning if pipe is found, if yes, call parsing with start/end */
void	scan_pipe(t_ast *ast, t_token *tokens, int *i)
{
	int			start;
	int			end;

	start = *i;
	end = last_token(tokens);
	if (tokens[start].word == NULL)
		return ;
	ast->nty = NODE_PIPE;
	while (tokens[*i].word)
	{
		if (tokens[*i].ty == TOKEN_PIPE)
		{
			parse_pipe(ast, tokens, start, *i);
			(*i)++;
			break ;
		}
		(*i)++;
	}
	if (has_pipe(tokens, *i, end))
		scan_tokens(ast->pipe.right, tokens, *i, end);
	else if (tokens[*i].word)
		parse_cmd(ast->pipe.right, tokens, *i, end);
	return ;
}

/*
TODO: REMOVE PRINTF DEBUGS (ADD ERROR CATCH)
TODO: ADD ERROR CATCHING
*/
void	parse_pipe(t_ast *ast, t_token *tokens, int start, int end)
{
	if (!(tokens && tokens[0].word))
		return ;
	printf("pipe node->ADD\n");
	printf("	start pipe tk: %d, end pipe tk: %d\n",
		start,
		end);
	ast->pipe.left = make_ast_node(NODE_CMD);
	ast->pipe.right = make_ast_node(NODE_UNKNOWN);
	if (!ast->pipe.left || !ast->pipe.right)
	{
		printf("*** ERROR *** Failed to allocate AST nodes\n");
		return ;
	}
	parse_cmd(ast->pipe.left, tokens, start, end);
	return ;
}
