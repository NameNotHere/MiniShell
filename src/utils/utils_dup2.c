/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dup2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:45:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/09 12:22:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	try_dup2_stdout(t_msh *sh, int *fd_out)
{
	if (*fd_out == STDOUT_FILENO)
		return ;
	if (dup2(*fd_out, STDOUT_FILENO) == -1)
	{
		msg_perr(E_DUP2);
		safe_close_fd(fd_out);
		exit_error_free(sh, E_REDIR_OUTPUT_FAILED);
	}
}

void	try_dup2_stdin(t_msh *sh, int *fd_in)
{
	if (*fd_in == STDIN_FILENO)
		return ;
	if (dup2(*fd_in, STDIN_FILENO) == -1)
	{
		msg_perr(E_DUP2);
		safe_close_fd(fd_in);
		exit_error_free(sh, E_REDIR_INPUT_FAILED);
	}
}

void	try_dup2(t_msh *sh, int *fd_in, int *fd_out)
{
	try_dup2_stdin(sh, fd_in);
	try_dup2_stdout(sh, fd_out);
}
