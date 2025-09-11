/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/11 13:13:28 by tda-roch         ###   ########.fr       */
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

int	execute_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	if (cmd->built_in == false
		|| (fd_out != STDOUT_FILENO || fd_in != STDIN_FILENO))
	{
		sh->last_pid = safe_fork_cmd(sh, &fd_in, &fd_out);
		if (sh->last_pid == -1)
			return (sh->exit_code);
		if (sh->last_pid == 0)
			execute_cmd_in_child(sh, fd_in, fd_out, cmd);
		safe_close_fd_out(&fd_out);
		return (EXIT_SUCCESS);
	}
	safe_close_fds(&fd_in, &fd_out);
	if (cmd->built_in == true)
		sh->exit_code = execute_builtin(sh, cmd);
	return (sh->exit_code);
}
