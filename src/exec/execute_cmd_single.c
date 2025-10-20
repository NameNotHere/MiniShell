/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_single.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:01:06 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 02:37:31 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		sh->exit_code = exec_single_builtin(sh, cmd);
	return (sh->exit_code);
}

/*
	Executes a single builtin command.
	Since the single builtin does not run on fork, it requires specific
	stdin/stdout redirection handling:
		Saves and restores the shell's original stdin/stdout
		to prevent corruption of the shell's input processing.
*/
int	exec_single_builtin(t_msh *sh, t_cmd *cmd)
{
	int	saved_fd_stdin;
	int	saved_fd_stdout;

	saved_fd_stdin = -1;
	saved_fd_stdout = -1;
	if (cmd->redir && !save_std_fds(&saved_fd_stdin, &saved_fd_stdout))
	{
		return (ret_exit_msg(sh, EXIT_FAILURE, "error: failed to save stdin/stdout\
			 for builtin redirection\n"));
	}
	if (execute_redirection(sh, cmd->redir))
		sh->exit_code = execute_builtin(sh, cmd);
	restore_std_fds(saved_fd_stdin, saved_fd_stdout);
	return (sh->exit_code);
}
