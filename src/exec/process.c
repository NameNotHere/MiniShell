/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 04:29:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	safe_close_fd_in(int *fd_in)
{
	if (*fd_in != STDIN_FILENO && *fd_in >= 0)
	{
		close(*fd_in);
		*fd_in = -1;
	}

}

void	safe_close_fd_out(int *fd_out)
{
	if (*fd_out != STDOUT_FILENO && *fd_out >= 0)
	{
		close(*fd_out);
		*fd_out = -1;
	}
}

void	exit_error(const char *error)
{
	if (errno)
		perror(error);
	else
		put_stderr_2(error, "\n");
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
	else
		exit(EXIT_FAILURE);
}

void	exit_error_free(t_msh *sh, const char *error)
{
	free_everything(sh);
	exit_error(error);
}

void	exit_free_with_code(t_msh *sh, int exit_code)
{
	free_everything(sh);
	exit(exit_code);
}

void	close_fds_exit_error_free(t_msh *sh, const char *error, int fd_in, int fd_out)
{
	safe_close_fd_in(&fd_in);
	safe_close_fd_out(&fd_out);
	exit_error_free(sh, error);
}

int	handle_execute_command_errors(t_msh *sh, t_cmd *cmd)
{
	if (errno == EACCES)
	{
		if (cmd->full_cmd == NULL)
			put_stderr("permission denied: (empty command)\n");
		else
			put_stderr_3("permission denied: ", cmd->argv[0], "\n");
		sh->exit_code = 126;
		return (126);
	}
	if (cmd->full_cmd == NULL)
		put_stderr("command not found: (empty command)\n");
	else
		put_stderr_3("command not found: ", cmd->argv[0], "\n");
	sh->exit_code = 127;
	return (127);
}

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
		safe_close_fd_in(&fd_in);
		safe_close_fd_out(&fd_out);
		perror("fork");
		sh->exit_code = errno;
		return (sh->exit_code);
	}
	if (pid == 0)
	{
		safe_close_fd_in(&fd_in);
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close_fds_exit_error_free(sh,
				"error: failed to redirect output", fd_in, fd_out);
		}
		safe_close_fd_out(&fd_out);
		sh->exit_code = execute_command(sh, cmd);
		exit_free_with_code(sh, sh->exit_code);
	}
	safe_close_fd_out(&fd_out);
	return (EXIT_SUCCESS);
}

// pipefd[1] is used by the left-side to write to the pipe (STDOUT_FILENO)
// pipefd[0] is used by the right-side to read from the pipe (STDIN_FILENO)
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

/*
This just prints a debug print to check redir received at execution
TODO: REMOVE THIS FUNCTION BEFORE EVAL
*/
void	debug_print_one_redir(t_redir *redir)
{
	if (redir && redir->string)
	{
		printf("redir type: %s string: |%s|\n",
			get_redir_symbol(redir->ty),
			redir->string);
		debug_print_one_redir(redir->next);
	}
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
