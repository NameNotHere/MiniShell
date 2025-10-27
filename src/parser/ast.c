/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/27 13:42:37 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
TODO: change return value for a custom one? (will count as exit code)
*/
int	build_ast(t_msh *sh, t_ast *ast, t_token *tokens)
{
	int	result;

	if (tokens == NULL || tokens[0].word == NULL)
		return (EXIT_SUCCESS);
	result = scan_tokens(sh, ast, 0, last_token(tokens));
	return (result);
}

int	scan_tokens(t_msh *sh, t_ast *ast, int start, int end)
{
	int		i;
	t_ast	*current_node;

	current_node = ast;
	i = start;
	if (validate_semicolon_syntax(sh->tokens) != 0)
		return (ret_exit_msg(sh, 2, E_SEMICOLON_MSG));
	if (has_pipe(sh->tokens, start, end))
	{
		if (validate_pipe_syntax(sh->tokens, start, end) != 0)
			return (ret_exit_msg(sh, 2,
					"minishell: syntax error near unexpected token `|'\n"));
		scan_pipe(sh, current_node, sh->tokens, &i);
	}
	else
		parse_cmd(sh, current_node, start, end);
	if (sh->exit_code != EXIT_SUCCESS)
		return (sh->exit_code);
	return (EXIT_SUCCESS);
}

/*scanning if pipe is found, if yes, call parsing with start/end */
void	scan_pipe(t_msh *sh, t_ast *ast, t_token *tokens, int *i)
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
			parse_pipe(sh, ast, start, *i);
			(*i)++;
			break ;
		}
		(*i)++;
	}
	if (has_pipe(tokens, *i, end))
		scan_tokens(sh, ast->pipe.right, *i, end);
	else if (tokens[*i].word)
		parse_cmd(sh, ast->pipe.right, *i, end);
	return ;
}

/*
TODO: REMOVE PRINTF DEBUGS (ADD ERROR CATCH)
TODO: ADD ERROR CATCHING
*/
void	parse_pipe(t_msh *sh, t_ast *ast, int start, int end)
{
	if (!(sh->tokens && sh->tokens[0].word))
		return ;
	ast->pipe.left = make_ast_node(NODE_CMD);
	ast->pipe.right = make_ast_node(NODE_UNKNOWN);
	if (!ast->pipe.left || !ast->pipe.right)
	{
		msg_err("Failed to allocate AST nodes\n");
		return ;
	}
	parse_cmd(sh, ast->pipe.left, start, end);
	return ;
}
