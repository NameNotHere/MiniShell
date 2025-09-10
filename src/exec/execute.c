/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/10 18:10:26 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// pipefd[1] for left-side to write to the pipe (STDOUT_FILENO)
// pipefd[0] for right-side to read from the pipe (STDIN_FILENO)
int	execute_pipe_node(t_msh *sh, t_pipe *pipe_node, int fd_in, int fd_out)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		sh->exit_code = errno;
		safe_close_fds(&fd_in, &fd_out);
		return (sh->exit_code);
	}
	left_pid = safe_fork_pipe(sh, pipefd, &fd_in, &fd_out);
	if (left_pid == -1)
		return (sh->exit_code);
	if (left_pid == 0)
	{
		try_dup2(sh, &fd_in, &pipefd[1]);
		safe_close_fds(&pipefd[0], &pipefd[1]);
		safe_close_fd_out(&fd_out);
		sh->exit_code = execute_command(sh, &pipe_node->left->cmd);
		exit_free_with_code(sh, sh->exit_code);
	}
	right_pid = safe_fork_pipe(sh, pipefd, &fd_in, &fd_out);
	if (right_pid == -1)
		return (sh->exit_code);
	if (right_pid == 0)
	{
		try_dup2(sh, &pipefd[0], &fd_out);
		safe_close_fds(&pipefd[0], &pipefd[1]);
		safe_close_fd_in(&fd_in);
		if (pipe_node->right->nty == NODE_CMD)
			sh->exit_code = execute_command(sh, &pipe_node->right->cmd);
		else
			sh->exit_code = execute_ast_node(sh, pipe_node->right,
					STDIN_FILENO, STDOUT_FILENO);
		exit_free_with_code(sh, sh->exit_code);
	}
	safe_close_fds(&pipefd[0], &pipefd[1]);
	safe_close_fds(&fd_in, &fd_out);
	sh->last_pid = right_pid;
	return (EXIT_SUCCESS);
}

int	execute_ast_node(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	if (!node)
	{
		put_stderr("error: on execute, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = execute_cmd_node(sh, &node->cmd, fd_in, fd_out);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = execute_pipe_node(sh, &node->pipe, fd_in, fd_out);
	return (sh->exit_code);
}

int	execute_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	int		child_status;
	pid_t	child_pid;
	int		last_exit_status;

	if (!node)
	{
		put_stderr("error: on execute, ast root node is NULL");
		return (EXIT_FAILURE);
	}
	last_exit_status = sh->exit_code;
	if (heredoc_ast_node(sh, sh->ast) != EXIT_SUCCESS)
	{
		perror("heredoc root ast node");
		return (sh->exit_code);
	}
	if (execute_ast_node(sh, sh->ast,
			fd_in, fd_out) != EXIT_SUCCESS)
	{
		last_exit_status = sh->exit_code;
		perror("execute root ast node");
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
	if (errno != ECHILD)  // silence no-child error
		sh->exit_code = last_exit_status;
	return (sh->exit_code);
}
