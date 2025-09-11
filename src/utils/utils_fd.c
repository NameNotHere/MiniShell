/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:28:55 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/11 13:05:36 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
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

void	safe_close_fds(int *fd_in, int *fd_out)
{
	safe_close_fd_in(fd_in);
	safe_close_fd_out(fd_out);
}

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
			safe_close_fds(fd_in, fd_out);
		sh->exit_code = errno;
	}
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
			safe_close_fds(&pipe_fds[0], &pipe_fds[1]);
		if (fd_in && fd_out)
			safe_close_fds(fd_in, fd_out);
		sh->exit_code = errno;
	}
	return (pid);
}

/*
** execute_in_child - Common child process execution pattern
** Handles: dup2, close fds, redirection, then calls exec_func
** exec_func should set sh->exit_code and NOT call exit
*/
void	execute_cmd_in_child(t_msh *sh, int fd_in, int fd_out, t_cmd *cmd)
{
	try_dup2(sh, &fd_in, &fd_out);
	safe_close_fds(&fd_in, &fd_out);
	sh->exit_code = execute_command(sh, cmd);
	exit_free_with_code(sh, sh->exit_code);
}


