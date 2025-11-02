/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:31:43 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hdoc_redir(t_msh *sh, t_redir *redir, int prev_hdoc_fd)
{
	char	*hdoc_str;
	int		write_fd;

	if (!redir)
		return ;
	if (redir && redir->ty != REDIR_HEREDOC)
		return (hdoc_redir(sh, redir->next, prev_hdoc_fd));
	safe_close_fd(&prev_hdoc_fd);
	write_fd = open("/tmp/tmp_hdoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (write_fd == -1)
		return (hdoc_err(sh, NULL, NULL, NULL), ms_perror("open hdoc"));
	redir->fd = open("/tmp/tmp_hdoc", O_RDONLY);
	if (redir->fd == -1)
		return (hdoc_err(sh, &write_fd, NULL, NULL), ms_perror("open hdoc"));
	unlink("/tmp/tmp_hdoc");
	hdoc_str = hdoc_loop(sh, redir);
	if (hdoc_str == NULL && g_sig == SIGINT)
		return (safe_close_2_fds(&write_fd, &redir->fd));
	if (hdoc_str == NULL && set_empty_string(&hdoc_str) == false)
		return (hdoc_err(sh, &write_fd, &redir->fd, hdoc_str), ms_perror("alloc"));
	if ((write(write_fd, hdoc_str, ft_strlen(hdoc_str)) == -1)
		|| (ft_strlen(hdoc_str) > 0 && write(write_fd, "\n", 1) == -1))
		return (hdoc_err(sh, &write_fd, &redir->fd, hdoc_str), ms_perror("write"));
	close(write_fd);
	safe_free_string(&hdoc_str);
	hdoc_redir(sh, redir->next, redir->fd);
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	hdoc_redir(sh, cmd->redir, 0);
	return (sh->exit_code);
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
		msg_err("heredoc_ast_node, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = heredoc_cmd_node(sh, &node->cmd);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = heredoc_pipe_node(sh, &node->pipe);
	return (sh->exit_code);
}
