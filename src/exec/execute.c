/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/08 17:32:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_msh *sh, t_cmd *cmd)
{
	if (cmd->not_found)
	{
		if (cmd->argv[0] == NULL)
			put_stderr("command not found: (empty command)\n");
		else
			put_stderr_3("command not found: ", cmd->argv[0], "\n");
		sh->exit_code = 127;
		return (127);
	}
	execve(cmd->full_cmd, cmd->argv, sh->envp);
	return (handle_execute_command_errors(sh, cmd));
}

int	execute_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	pid_t	pid;

	sh->last_pid = fork();
	pid = sh->last_pid;
	if (pid == -1)
	{
		safe_close_fds(&fd_in, &fd_out);
		perror("fork");
		sh->exit_code = errno;
		return (sh->exit_code);
	}
	if (pid == 0)
	{
		try_dup2(sh, &fd_in, &fd_out);
		safe_close_fds(&fd_in, &fd_out);
		sh->exit_code = execute_command(sh, cmd);
		exit_free_with_code(sh, sh->exit_code);
	}
	debug_print_one_redir(cmd->redir);
	safe_close_fd_out(&fd_out);
	return (EXIT_SUCCESS);
}

// pipefd[1] for left-side to write to the pipe (STDOUT_FILENO)
// pipefd[0] for right-side to read from the pipe (STDIN_FILENO)
int	execute_pipe_node(t_msh *sh, t_pipe *pipe_node, int fd_in, int fd_out)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		sh->exit_code = errno;
		return (sh->exit_code);
	}
	sh->exit_code = execute_cmd_node(sh, &pipe_node->left->cmd,
			fd_in, pipefd[1]);
	safe_close_fd_out(&pipefd[1]);
	if (sh->exit_code)
		return (sh->exit_code);
	sh->exit_code = execute_ast_node(sh, pipe_node->right, pipefd[0], fd_out);
	safe_close_fd_in(&pipefd[0]);
	return (sh->exit_code);
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
	sh->exit_code = last_exit_status;
	return (sh->exit_code);
}
