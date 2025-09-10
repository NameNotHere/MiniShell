/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/10 11:06:03 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

char	*heredoc_loop(t_redir *redir)
{
	char	*hdoc_line;
	char	*hdoc_string;
	char	*temp_hdoc_string;

	hdoc_string = NULL;
	while (true)
	{
		hdoc_line = readline("hdoc > ");
		if (!hdoc_line || !*hdoc_line)
		{
			safe_free_string(&hdoc_line);
			continue ;
		}
		if (ft_strncmp(redir->string, hdoc_line, ft_strlen(redir->string)) == 0)
			break ;
		if (hdoc_string)
			temp_hdoc_string = ft_strjoin3(hdoc_string, "\n", hdoc_line);
		else
			temp_hdoc_string = hdoc_line;
		safe_free_string(&hdoc_string);
		hdoc_string = temp_hdoc_string;
		temp_hdoc_string = NULL;
		if (hdoc_string == NULL)
		{
			perror("heredoc_loop allocation");
			break ;
		}
	}
	// if (hdoc_string != NULL)
	// 	temp_print("heredoc string is:\n%s", hdoc_string);
	return (hdoc_string);
}

void	heredoc_redirection(t_msh *sh, t_redir *redir, int previous_hdoc_fd)
{
	char	*hdoc_string;

	if (redir && redir->ty == REDIR_HEREDOC)
	{
		temp_print("heredoc found\n");
		if (previous_hdoc_fd)
			close(previous_hdoc_fd);
		redir->fd = open("/tmp/myshell_tmp_heredoc",
				O_RDWR | O_CREAT | O_TRUNC, 0600);
		if (redir->fd == -1)
			return (perror("open myshell_tmp_heredoc"));
		unlink("/tmp/myshell_tmp_heredoc");
		hdoc_string = heredoc_loop(redir);
		if (hdoc_string == NULL)
		{
			sh->exit_code = EXIT_FAILURE;
			perror("heredoc failed");
			return ;
		}
		if (write(redir->fd, hdoc_string, ft_strlen(hdoc_string)) == -1)
		{
			close(redir->fd);
			safe_free_string(&hdoc_string);
			sh->exit_code = EXIT_FAILURE;
			perror("heredoc failed");
			return ;
		}
		safe_free_string(&hdoc_string);
		heredoc_redirection(sh, redir->next, redir->fd);
	}
	else if (redir)
		heredoc_redirection(sh, redir->next, previous_hdoc_fd);
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	heredoc_redirection(sh, cmd->redir, 0);
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
