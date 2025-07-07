/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:18:05 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 22:13:14 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_redir_symbol(t_redir_ty ty)
{
	if (ty == REDIR_INPUT)
		return (REDIR_INPUT_PRINT);
	if (ty == REDIR_OUTPUT)
		return (REDIR_OUTPUT_PRINT);
	if (ty == REDIR_APPEND)
		return (REDIR_APPEND_PRINT);
	if (ty == REDIR_HEREDOC)
		return (REDIR_HEREDOC_PRINT);
	return (REDIR_INPUT_PRINT);
}

/*
TODO: REMOVE THIS WHEN FINISHED DEBUGGING, BEFORE SUBMITTING!
MAYBE ADD PRINT AST FUNCTIONS TO A SEPARATE TEST SUITE
*/
void	print_ast(t_ast *root)
{
	if (!root)
	{
		a_print("\n\nAST: (empty)\n");
		return ;
	}
	a_print("\n\nAST:\n");
	print_ast_node(root, 0);
}

/*
Prints representation (very simplified) of the AST tree
NOTE: Uses indentation (updating depth var) to represent tree structure
	- first item at each depth is either ROOT, or LEFT node
	- second item at each depth is RIGHT node
TODO: REMOVE THIS WHEN FINISHED DEBUGGING, BEFORE SUBMITTING!
MAYBE ADD PRINT AST FUNCTIONS TO A SEPARATE TEST SUITE
*/
void	print_ast_node(t_ast *node, int depth)
{
	int	i;

	if (!node)
		return ;
	i = -1;
	while (++i < depth)
		a_print("  ");
	if (node->nty == NODE_CMD)
		print_ast_cmd(node);
	else if (node->nty == NODE_PIPE)
	{
		a_print("PIPE\n");
		print_ast_node(node->pipe.left, depth + 1);
		print_ast_node(node->pipe.right, depth + 1);
	}
}

void	print_ast_cmd(t_ast *node)
{
	int		i;
	t_redir	*redir;

	a_print("CMD: ");
	if (node->cmd.argv && node->cmd.argv[0])
	{
		i = -1;
		while (node->cmd.argv[++i])
		{
			a_print("%s", node->cmd.argv[i]);
			if (node->cmd.argv[i + 1])
				a_print(" ");
		}
	}
	redir = node->cmd.redir;
	while (redir)
	{
		a_print(" [%s: %s]", get_redir_symbol(redir->ty),
			redir->string);
		redir = redir->next;
	}
	a_print("\n");
}

/*
TODO: REMOVE TEST BEFORE EVALUATION
*/
int	print_build_ast(t_msh *sh,  t_ast *ast, t_token *tokens)
{
	if (!ast)
		return (EXIT_SUCCESS);
	build_ast(sh, ast, tokens);
	if (DEBUG_MINISHELL)
		print_ast(ast);
	return (EXIT_SUCCESS);
}
