/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 16:00:42 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	build_ast(t_msh *sh, t_ast *ast, t_token *tokens)
{
	int	ret;

	if (tokens == NULL || tokens[0].word == NULL)
		return (EXIT_SUCCESS);
	ret = scan_tokens(sh, ast, 0, last_token(tokens));
	return (ret);
}

int	scan_tokens(t_msh *sh, t_ast *ast, int start, int end)
{
	int		i;
	int		ret;
	t_ast	*current_node;

	current_node = ast;
	i = start;
	if (VALIDATE && detect_unsupported_operator(sh->tokens))
		return (process_unsupported_operator_error(sh));
	if (has_pipe(current_node, sh->tokens, start, end))
	{
		if (validate_pipe_syntax(sh->tokens, start, end) != 0)
			return (r_msg_err(E_SYNTAX_PIPE, EXIT_SYNTAX));
		ret = scan_pipe(sh, current_node, sh->tokens, &i);
	}
	else
		ret = parse_cmd(sh, current_node, start, end);
	return (ret);
}

/*scanning if pipe is found, if yes, call parsing with start/end */
int	scan_pipe(t_msh *sh, t_ast *ast, t_token *tokens, int *i)
{
	int			start;
	int			end;
	int			ret;

	start = *i;
	end = last_token(tokens);
	if (tokens[start].word == NULL)
		return (EXIT_SUCCESS);
	while (tokens[*i].word)
	{
		if (tokens[*i].ty == TOKEN_PIPE)
		{
			ret = parse_pipe(sh, ast, start, *i);
			if (ret != EXIT_SUCCESS)
				return (ret);
			(*i)++;
			break ;
		}
		(*i)++;
	}
	if (has_pipe(ast->pipe.right, tokens, *i, end))
		return (scan_pipe(sh, ast->pipe.right, tokens, i));
	else if (tokens[*i].word)
		return (parse_cmd(sh, ast->pipe.right, *i, end));
	return (EXIT_SUCCESS);
}

int	parse_pipe(t_msh *sh, t_ast *ast, int start, int end)
{
	if (!(sh->tokens && sh->tokens[0].word))
		return (EXIT_SUCCESS);
	ast->pipe.left = make_ast_node(NODE_CMD);
	ast->pipe.right = make_ast_node(NODE_UNKNOWN);
	if (!ast->pipe.left || !ast->pipe.right)
		return (EXIT_FAILURE);
	return (parse_cmd(sh, ast->pipe.left, start, end));
}
