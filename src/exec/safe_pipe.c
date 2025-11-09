/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 22:10:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/09 12:22:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	safe_pipe(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out)
{
	if (pipe(pipefd) == -1)
	{
		set_exit_perr(sh, E_PIPE);
		safe_close_2_fds(fd_in, fd_out);
		return (false);
	}
	return (true);
}
