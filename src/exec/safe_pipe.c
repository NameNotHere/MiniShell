/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 22:10:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/12 23:14:08 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	safe_pipe(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		sh->exit_code = errno;
		safe_close_fds(fd_in, fd_out);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
