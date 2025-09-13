/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:28:55 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/12 23:52:16 by tda-roch         ###   ########.fr       */
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

int	cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out)
{
	safe_close_fds(&pipefd[0], &pipefd[1]);
	safe_close_fds(fd_in, fd_out);
	return (sh->exit_code);
}
