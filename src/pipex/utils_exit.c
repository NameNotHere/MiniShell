/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:38:26 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/14 03:20:01 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "pipex.h"

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

void	exit_error_free(t_pipex *px, const char *error)
{
	free_everything(px);
	exit_error(error);
}

void	exit_free_with_code(t_pipex *px, int exit_code)
{
	free_everything(px);
	exit(exit_code);
}

void	close_fds_exit_error_free(t_pipex *px, const char *error, int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
	exit_error_free(px, error);
}
