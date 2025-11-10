/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:17:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 12:49:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(const char *error)
{
	if (errno)
		msg_perr(error);
	else
		msg_err(error);
	if (errno == EACCES)
		exit(EXIT_PERM_DENIED);
	else if (errno == ENOENT)
		exit(EXIT_CMD_NOT_FOUND);
	else
		exit(EXIT_FAILURE);
}

void	exit_error_free(t_msh *sh, const char *error)
{
	free_everything(sh);
	exit_error(error);
}

void	exit_free_with_code(t_msh *sh, int exit_code)
{
	free_everything(sh);
	exit(exit_code);
}

void	close_fds_exit_error_free(t_msh *sh, const char *error,\
	int *fd_in, int *fd_out)
{
	safe_close_2_fds(fd_in, fd_out);
	exit_error_free(sh, error);
}

int	handle_execute_command_errors(t_cmd *cmd)
{
	(void)cmd;
	if (errno == EACCES)
	{
		if (cmd->full_cmd == NULL)
			msg_err(E_PERM_DENIED_EMPTY);
		else
			msg_err_2(E_PERM_DENIED, cmd->full_cmd);
		return (EXIT_PERM_DENIED);
	}
	if (cmd->full_cmd == NULL)
		msg_err(E_CMD_NOT_FOUND_EMPTY);
	else
		msg_err_2(E_CMD_NOT_FOUND, cmd->argv[0]);
	return (EXIT_CMD_NOT_FOUND);
}
