/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 07:10:38 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_children(t_msh *sh);
static void	process_child_status(t_msh *sh, pid_t child_pid, int child_status);

// pipefd[1] for left-side to write to the pipe (STDOUT_FILENO)
// pipefd[0] for right-side to read from the pipe (STDIN_FILENO)
int	exec_pipe_node(t_msh *sh, t_pipe *pipe_node, int fd_in, int fd_out)
{
	int		pipefd[2];

	if (!safe_pipe(sh, pipefd, &fd_in, &fd_out))
		return (sh->exit_code);
	if (safe_fork_pipe(sh, pipefd, &fd_in, &fd_out) == 0)
		exec_left(sh, pipe_node->left, pipefd, (int *[2]){&fd_in, &fd_out});
	if (sh->exit_code != EXIT_SUCCESS)
		return (cleanup_all_fds(sh, pipefd, &fd_in, &fd_out));
	safe_close_fd(&pipefd[1]);
	if (pipe_node->right->nty == NODE_PIPE)
	{
		safe_close_fd(&fd_in);
		return (exec_pipe_node(sh, &pipe_node->right->pipe, pipefd[0], fd_out));
	}
	if (safe_fork_pipe(sh, pipefd, &fd_in, &fd_out) == 0)
		exec_right(sh, pipe_node->right, pipefd, (int *[2]){&fd_in, &fd_out});
	return (cleanup_all_fds(sh, pipefd, &fd_in, &fd_out));
}

int	exec_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	if (!node)
		return (r_msg_err(E_AST_ROOT_NULL, EXIT_FAILURE));
	if (!set_ignore_sig())
		return (r_set_exit_msg(sh, EXIT_FAILURE, E_AST_ROOT_SIG));
	if (node->nty == NODE_CMD)
		sh->exit_code = exec_single_cmd_node(sh, &node->cmd, fd_in, fd_out);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = exec_pipe_node(sh, &node->pipe, fd_in, fd_out);
	return (sh->exit_code);
}

int	exec_ast(t_msh *sh, t_ast *node, int fd_in, int fd_out)
{
	sh->last_pid = 0;
	if (!node)
		return (r_msg_err(E_EXEC_AST_NULL, EXIT_FAILURE));
	if (heredoc_ast_node(sh, sh->ast) != EXIT_SUCCESS)
		return (sh->exit_code);
	sh->exit_code = exec_ast_root(sh, sh->ast, fd_in, fd_out);
	if (sh->last_pid != 0)
		return (wait_children(sh));
	return (sh->exit_code);
}

/*
Process the exit status of a child and update shell exit code if it's the last
	child.
*/
static void	process_child_status(t_msh *sh, pid_t child_pid, int child_status)
{
	if (child_pid == sh->last_pid)
	{
		if (WIFEXITED(child_status))
			sh->exit_code = WEXITSTATUS(child_status);
		else if (WIFSIGNALED(child_status))
			sh->exit_code = 128 + WTERMSIG(child_status);
	}
}

/*
	Reap all children and update the shell exit code from the rightmost child.
	Returns the updated exit code.
*/
static int	wait_children(t_msh *sh)
{
	int		child_status;
	pid_t	child_pid;
	bool	interrupted;

	interrupted = false;
	while (true)
	{
		child_pid = wait(&child_status);
		if (WIFSIGNALED(child_status)
			&& (WTERMSIG(child_status) == SIGINT || WCOREDUMP(child_status)))
			interrupted = true;
		if (child_pid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno != ECHILD)
				msg_perr(E_WAIT);
			break ;
		}
		process_child_status(sh, child_pid, child_status);
	}
	if (interrupted)
		(void)write(STDERR_FILENO, "\n", 1);
	return (sh->exit_code);
}
