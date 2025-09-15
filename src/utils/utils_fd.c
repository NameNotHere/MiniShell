/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:28:55 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/13 20:38:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	Closes open fds when they are not STDIN, STDOUT or STDERR
*/
void	safe_close_fd(int *fd)
{
	int	fd_value;

	if (!fd || *fd <= STDERR_FILENO)
		return ;
	fd_value = *fd;
	*fd = -1;
	close(fd_value);
}

void	safe_close_2_fds(int *fd_one, int *fd_two)
{
	safe_close_fd(fd_one);
	safe_close_fd(fd_two);
}

int	cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out)
{
	safe_close_2_fds(&pipefd[0], &pipefd[1]);
	safe_close_2_fds(fd_in, fd_out);
	return (sh->exit_code);
}
