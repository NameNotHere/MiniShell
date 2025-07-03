/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:04:16 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/29 18:29:34 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <errno.h>

t_ast	*make_ast_node(t_node_ty type)
{
	t_ast	*new_node;

	new_node = ft_calloc(1, sizeof(t_ast));
	if (!new_node)
		return (NULL);
	new_node->nty = type;
	return (new_node);
}

void	free_ast_cmd(t_ast *node)
{
	int		i;
	t_redir	*redir;
	t_redir	*next;

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
void	free_ast(t_ast **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->nty == NODE_CMD)
		free_ast_cmd(*node);
	else if ((*node)->nty == NODE_PIPE)
	{
		free_ast(&(*node)->pipe.left);
		free_ast(&(*node)->pipe.right);
	}
	free(*node);
	*node = NULL;  // This properly sets the caller's pointer to NULL
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
