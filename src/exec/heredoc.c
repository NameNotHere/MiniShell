/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/09 09:47:18 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

void	heredoc_redirection(t_msh *sh, t_redir *redir)
{
	char	*heredoc_string;

	if (redir && redir->ty == REDIR_HEREDOC)
	{
		temp_print("heredoc found\n");
		redir->fd = open("/tmp/myshell_tmp_heredoc",
				O_RDWR | O_CREAT | O_TRUNC, 0600);
		if (redir->fd == -1)
			return (perror("open myshell_tmp_heredoc"));
		unlink("/tmp/myshell_tmp_heredoc");
		heredoc_string = readline("heredoc >");
		write(redir->fd, heredoc_string, ft_strlen(heredoc_string));
		// close(redir->fd);
		heredoc_redirection(sh, redir->next);
	}
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	heredoc_redirection(sh, cmd->redir);
	return (EXIT_SUCCESS);
}

int	heredoc_pipe_node(t_msh *sh, t_pipe *pipe_node)
{
	sh->exit_code = heredoc_cmd_node(sh, &pipe_node->left->cmd);
	if (sh->exit_code)
		return (sh->exit_code);
	sh->exit_code = heredoc_ast_node(sh, pipe_node->right);
	return (sh->exit_code);
}

int	heredoc_ast_node(t_msh *sh, t_ast *node)
{
	if (!node)
	{
		put_stderr("error: on execute_ast_node_heredoc, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = heredoc_cmd_node(sh, &node->cmd);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = heredoc_pipe_node(sh, &node->pipe);
	return (sh->exit_code);
}
