/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 07:34:40 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "minishell.h"

static void	close_all_hdocs(t_redir *redir, int *write_fd)
{
	if (!redir)
		return (safe_close_fd(write_fd));
	safe_close_2_fds(write_fd, &redir->fd);
	redir = redir->next;
	while (redir)
	{
		if (redir->ty == REDIR_HEREDOC)
			safe_close_fd(&redir->fd);
		redir = redir->next;
	}
}

void	hdoc_redir(t_msh *sh, t_redir *redir)
{
	char	*hdoc_str;
	int		tmp_fd;
	int		dup_fd;

	if (!redir)
		return ;
	if (redir && redir->ty != REDIR_HEREDOC)
		return (hdoc_redir(sh, redir->next));
	tmp_fd = open("/tmp", O_TMPFILE | O_RDWR, 0600);
	if (tmp_fd == -1)
		return (hdoc_err(sh, NULL, NULL, NULL), msg_perr(E_OPEN_HEREDOC));
	hdoc_str = hdoc_loop(sh, redir);
	if (hdoc_str == NULL && g_sig == SIGINT)
		return (close_all_hdocs(redir->next, &tmp_fd));
	if (hdoc_str == NULL && set_empty_string(&hdoc_str) == false)
		return (hdoc_err(sh, &tmp_fd, NULL, hdoc_str), msg_perr(E_ALLOC));
	if ((write(tmp_fd, hdoc_str, ft_strlen(hdoc_str)) == -1)
		|| (ft_strlen(hdoc_str) > 0 && write(tmp_fd, "\n", 1) == -1))
		return (hdoc_err(sh, &tmp_fd, NULL, hdoc_str), msg_perr(E_WRITE));
	if (lseek(tmp_fd, 0, SEEK_SET) == -1)
		return (hdoc_err(sh, &tmp_fd, NULL, hdoc_str), msg_perr(E_WRITE));
	dup_fd = fcntl(tmp_fd, F_DUPFD, 50);
	if (dup_fd == -1)
		return (hdoc_err(sh, &tmp_fd, NULL, hdoc_str), msg_perr(E_ALLOC));
	safe_close_fd(&tmp_fd);
	redir->fd = dup_fd;
	safe_free_str(&hdoc_str);
	hdoc_redir(sh, redir->next);
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	hdoc_redir(sh, cmd->redir);
	return (sh->exit_code);
}

int	heredoc_pipe_node(t_msh *sh, t_pipe *pipe_node)
{
	if (pipe_node->left->nty == NODE_CMD)
		sh->exit_code = heredoc_cmd_node(sh, &pipe_node->left->cmd);
	else if (pipe_node->left->nty == NODE_PIPE)
		sh->exit_code = heredoc_pipe_node(sh, &pipe_node->left->pipe);
	if (sh->exit_code)
		return (sh->exit_code);
	sh->exit_code = heredoc_ast_node(sh, pipe_node->right);
	return (sh->exit_code);
}

int	heredoc_ast_node(t_msh *sh, t_ast *node)
{
	if (!node)
	{
		msg_err(E_HEREDOC_AST_NULL);
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = heredoc_cmd_node(sh, &node->cmd);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = heredoc_pipe_node(sh, &node->pipe);
	return (sh->exit_code);
}
