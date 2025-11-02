/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:31:30 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	execute command, or just redirections.
	if this applies:
		(if cmd->argv[0] == NULL || !ft_strlen(cmd->argv[0])
		it means we have no command to run, just redirections. it should
		not fail in that case.
*/
int	execute_command(t_msh *sh, t_cmd *cmd)
{
	errno = 0;
	set_restore_dfl_sig();
	if (!execute_redirection(sh, cmd->redir))
		return (sh->exit_code);
	if (cmd->argv[0] == NULL)
		return (EXIT_SUCCESS);
	if (!ft_strlen(cmd->argv[0]))
	{
		msg_err("command not found: ");
		return (127);
	}
	if (cmd->built_in)
		return (execute_builtin(sh, cmd));
	if (cmd->is_a_dir)
		return (msg_err_2("is a directory: ", cmd->argv[0]), 126);
	if (cmd->permission_denied)
		return (msg_err_2("permission denied: ", cmd->argv[0]), 126);
	if (cmd->not_found)
		return (msg_err_2("command not found: ", cmd->argv[0]), 127);
	execve(cmd->full_cmd, cmd->argv, sh->envp);
	return (handle_execute_command_errors(cmd));
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
