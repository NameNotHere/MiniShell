/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/13 20:37:32 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_msh *sh, t_cmd *cmd)
{
	execute_redirection(sh, cmd->redir);
	if (cmd->built_in)
		return (execute_builtin(sh, cmd));
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

void	exec_single_cmd_in_child(t_msh *sh, int fd_in, int fd_out, t_cmd *cmd)
{
	try_dup2(sh, &fd_in, &fd_out);
	safe_close_2_fds(&fd_in, &fd_out);
	sh->exit_code = execute_command(sh, cmd);
	exit_free_with_code(sh, sh->exit_code);
}

int	exec_single_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	if (cmd->built_in == false
		|| (fd_out != STDOUT_FILENO || fd_in != STDIN_FILENO))
	{
		if (safe_fork_cmd(sh, &fd_in, &fd_out) == 0)
			exec_single_cmd_in_child(sh, fd_in, fd_out, cmd);
		safe_close_fd(&fd_out);
		return (sh->exit_code);
	}
	safe_close_2_fds(&fd_in, &fd_out);
	if (cmd->built_in == true)
		sh->exit_code = execute_builtin(sh, cmd);
	return (sh->exit_code);
}

void	exec_left(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2])
{
	int	*fd_in;
	int	*fd_out;

	fd_in = fd_in_out[0];
	fd_out = fd_in_out[1];
	try_dup2(sh, fd_in, &pipefd[1]);
	safe_close_2_fds(&pipefd[0], &pipefd[1]);
	safe_close_fd(fd_out);
	sh->exit_code = execute_command(sh, &node->cmd);
	exit_free_with_code(sh, sh->exit_code);
}

void	exec_right(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2])
{
	int	*fd_in;
	int	*fd_out;

	fd_in = fd_in_out[0];
	fd_out = fd_in_out[1];
	try_dup2(sh, &pipefd[0], fd_out);
	safe_close_2_fds(&pipefd[0], &pipefd[1]);
	safe_close_fd(fd_in);
	sh->exit_code = execute_command(sh, &node->cmd);
	exit_free_with_code(sh, sh->exit_code);
}
