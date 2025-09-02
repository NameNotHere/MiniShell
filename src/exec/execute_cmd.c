/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/09 01:31:58 by tda-roch         ###   ########.fr       */
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
		execute_redirection(sh, cmd->redir);
		if (cmd->built_in == false)
			sh->exit_code = execute_command(sh, cmd);
		else
			sh->exit_code = execute_built_in(sh, cmd);
		exit_free_with_code(sh, sh->exit_code);
	}
	debug_print_one_redir(cmd->redir);
	safe_close_fd_out(&fd_out);
	return (EXIT_SUCCESS);
}
