/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/13 20:34:42 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pipefd[1] for left-side to write to the pipe (STDOUT_FILENO)
// pipefd[0] for right-side to read from the pipe (STDIN_FILENO)
int	exec_pipe_node(t_msh *sh, t_pipe *pipe_node, int fd_in, int fd_out)
{
	int		pipefd[2];

	if (safe_pipe(sh, pipefd, &fd_in, &fd_out) == EXIT_FAILURE)
		return (sh->exit_code);
	if (safe_fork_pipe(sh, pipefd, &fd_in, &fd_out) == 0)
		exec_left(sh, pipe_node->left, pipefd, (int *[2]){&fd_in, &fd_out});
	if (sh->exit_code != EXIT_SUCCESS)
		return (cleanup_all_fds(sh, pipefd, &fd_in, &fd_out));
	if (pipe_node->right->nty == NODE_PIPE)
	{
		safe_close_fd(&fd_in);
		close(pipefd[1]);
		return (exec_pipe_node(sh, &pipe_node->right->pipe, pipefd[0], fd_out));
	}
	if (safe_fork_pipe(sh, pipefd, &fd_in, &fd_out) == 0)
		exec_right(sh, pipe_node->right, pipefd, (int *[2]){&fd_in, &fd_out});
	return (cleanup_all_fds(sh, pipefd, &fd_in, &fd_out));
}

int	exec_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	if (!node)
	{
		put_stderr("error: on exec, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = exec_single_cmd_node(sh, &node->cmd, fd_in, fd_out);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = exec_pipe_node(sh, &node->pipe, fd_in, fd_out);
	return (sh->exit_code);
}

int	exec_ast(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	int		child_status;
	pid_t	child_pid;
	int		last_exit_status;

	if (!node)
	{
		put_stderr("error: on exec, ast root node is NULL");
		return (EXIT_FAILURE);
	}
	last_exit_status = sh->exit_code;
	if (heredoc_ast_node(sh, sh->ast) != EXIT_SUCCESS)
	{
		perror("heredoc root ast node");
		return (sh->exit_code);
	}
	if (exec_ast_root(sh, sh->ast,
			fd_in, fd_out) != EXIT_SUCCESS)
	{
		last_exit_status = sh->exit_code;
		perror("exec root ast node");
	}
	child_pid = 1;
	while (child_pid > 0)
	{
		child_pid = wait(&child_status);
		if (child_pid == sh->last_pid)
		{
			if (WIFEXITED(child_status))
				last_exit_status = WEXITSTATUS(child_status);
			else if (WIFSIGNALED(child_status))
				last_exit_status = 128 + WTERMSIG(child_status);
		}
	}
	if (child_pid == -1 && (errno != ECHILD && errno != EINTR))
		perror("wait error");
	if (errno != ECHILD)
		sh->exit_code = last_exit_status;
	return (sh->exit_code);
}
