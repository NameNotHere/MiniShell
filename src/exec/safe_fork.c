/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:07:31 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 09:23:37 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	safe_fork_cmd - fork wrapper for cmd node fork with error handling
	Returns:
		child PID on success (parent), 0 in child, -1 on error
	Note: Handles error cleanup and sets exit_code on failure
*/
pid_t	safe_fork_cmd(t_msh *sh, int *fd_in, int *fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("cmd node fork");
		if (fd_in && fd_out)
			safe_close_2_fds(fd_in, fd_out);
		sh->exit_code = errno;
	}
	sh->last_pid = pid;
	return (pid);
}

/*
	safe_fork_pipe - Fork wrapper for pipe node with extra cleanup
	Returns:
		child PID on success (parent), 0 in child, -1 on error
*/
pid_t	safe_fork_pipe(t_msh *sh, int *pipe_fds, int *fd_in, int *fd_out)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("pipe node fork");
		if (pipe_fds)
			safe_close_2_fds(&pipe_fds[0], &pipe_fds[1]);
		if (fd_in && fd_out)
			safe_close_2_fds(fd_in, fd_out);
		sh->exit_code = errno;
		return (pid);
	}
	if (pid == 0)
	{
		sh->exit_code = EXIT_SUCCESS;
		set_ignore_sigpipe();
	}
	sh->last_pid = pid;
	return (pid);
}
