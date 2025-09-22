/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:04:16 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/21 02:28:51 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// #include <errno.h>

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
	t_redir	*redir;
	t_redir	*next;

	safe_free_2d_string(&node->cmd.argv);
	safe_free_string(&node->cmd.full_cmd);
	redir = node->cmd.redir;
	while (redir)
	{
		next = redir->next;
		safe_free_string(&redir->string);
		safe_free((void **)&redir);
		redir = next;
	}
}

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
	safe_free((void **)node);
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
