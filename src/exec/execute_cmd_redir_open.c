/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_redir_open.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:27:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/09 02:26:35 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	failed_open_to_null(t_msh *sh, char *filename, int o_flag)
{
	int	fd;

	perror(filename);
	fd = open("/dev/null", o_flag);
	if (fd == -1)
		exit_error_free(sh, "failed to open /dev/null");
	sh->exit_code = EXIT_FAILURE;
	return (fd);
}


int	open_input_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		fd = failed_open_to_null(sh, filename, O_RDONLY);
	return (fd);
}

int	open_output_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_PERMISSIONS);
	if (fd == -1)
		fd = failed_open_to_null(sh, filename, O_WRONLY);
	return (fd);
}

int	open_append_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, OUTPUT_PERMISSIONS);
	if (fd == -1)
		fd = failed_open_to_null(sh, filename, O_WRONLY);
	return (fd);
}
