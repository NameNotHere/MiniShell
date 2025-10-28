/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:17:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/25 16:37:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(const char *error)
{
	if (errno)
		perror(error);
	else
		msg_err_2(error, "\n");
	if (errno == EACCES)
		exit(126);
	else if (errno == ENOENT)
		exit(127);
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
			msg_err("permission denied: (empty command)\n");
		else
			msg_err_3("permission denied: ", cmd->full_cmd, "\n");
		return (126);
	}
	if (cmd->full_cmd == NULL)
		msg_err("command not found: (empty command)\n");
	else
		msg_err_3("command not found: ", cmd->argv[0], "\n");
	return (127);
}
