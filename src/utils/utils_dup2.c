/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dup2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:45:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/08 16:51:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	try_dup2_stdout(t_msh *sh, int *fd_in, int *fd_out)
{
	if (*fd_out == STDOUT_FILENO)
		return ;
	if (dup2(*fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close_fds_exit_error_free(sh,
			"error: failed to redirect output", fd_in, fd_out);
	}
}

void	try_dup2_stdin(t_msh *sh, int *fd_in, int *fd_out)
{
	if (*fd_in == STDIN_FILENO)
		return ;
	if (dup2(*fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close_fds_exit_error_free(sh,
			"error: failed to redirect input", fd_in, fd_out);
	}
}

void	try_dup2(t_msh *sh, int *fd_in, int *fd_out)
{
	try_dup2_stdin(sh, fd_in, fd_out);
	try_dup2_stdout(sh, fd_in, fd_out);
}
