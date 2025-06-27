/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:18:05 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/27 05:55:51 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
TODO: REMOVE THIS WHEN FINISHED DEBUGGING, BEFORE SUBMITTING!
MAYBE ADD PRINT AST FUNCTIONS TO A SEPARATE TEST SUITE
*/
void	print_ast(t_ast *root)
{
	if (!root)
	{
		printf("\n\nAST: (empty)\n");
		return ;
	}
	printf("\n\nAST:\n");
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
		printf("  ");
	if (node->nty == NODE_CMD)
		print_ast_cmd(node);
	else if (node->nty == NODE_PIPE)
	{
		printf("PIPE\n");
		print_ast_node(node->pipe.left, depth + 1);
		print_ast_node(node->pipe.right, depth + 1);
	}
}

void	print_ast_cmd(t_ast *node)
{
	int				i;
	t_redir	*redir;

	printf("CMD: ");
	if (node->cmd.argv && node->cmd.argv[0])
	{
		i = -1;
		while (node->cmd.argv[++i])
		{
			printf("%s", node->cmd.argv[i]);
			if (node->cmd.argv[i + 1])
				printf(" ");
		}
	}
	redir = node->cmd.redir;
	while (redir)
	{
		printf(" [%s%s]", get_redir_symbol(redir->ty),
			redir->string);
		redir = redir->next;
	}
	printf("\n");
}

char	*get_redir_symbol(t_redir_ty ty)
{
	if (ty == REDIR_INPUT)
		return (REDIR_INPUT_SYMBOL);
	if (ty == REDIR_OUTPUT)
		return (REDIR_OUTPUT_SYMBOL);
	if (ty == REDIR_APPEND)
		return (REDIR_APPEND_SYMBOL);
	if (ty == REDIR_HEREDOC)
		return (REDIR_HEREDOC_SYMBOL);
	return (REDIR_INPUT_SYMBOL);
}
