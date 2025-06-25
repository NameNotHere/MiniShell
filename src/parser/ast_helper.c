/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:04:16 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 04:55:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <errno.h>

t_ast_node	*make_ast_node(t_node_type type)
{
	t_ast_node	*new_node;

	new_node = ft_calloc(1, sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->nty = type;
	return (new_node);
}

void	free_ast_cmd(t_ast_node *node)
{
	int				i;
	t_redir_node	*redir;
	t_redir_node	*next;

	if (node->cmd.argv)
	{
		i = -1;
		while (node->cmd.argv[++i])
			free(node->cmd.argv[i]);
		free(node->cmd.argv);
	}
	redir = node->cmd.redir;
	while (redir)
	{
		next = redir->next;
		free(redir->string);
		free(redir);
		redir = next;
	}
}

/*
TODO: REMOVE THIS WHEN FINISHED DEBUGGING, BEFORE SUBMITTING!
MAYBE ADD PRINT AST FUNCTIONS TO A SEPARATE TEST SUITE
*/
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->nty == NODE_CMD)
		free_ast_cmd(node);
	else if (node->nty == NODE_PIPE)
	{
		free_ast(node->pipe.left);
		free_ast(node->pipe.right);
	}
	free(node);
}

int	last_token(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].word)
		i++;
	return (i);
}

bool	has_pipe(t_token *tokens, int start, int end)
{
	int	i;

	i = start;
	while (tokens[i].word && i <= end)
	{
		if (tokens[i].ty == TOKEN_PIPE)
			return (true);
		i++;
	}
	return (false);
}
