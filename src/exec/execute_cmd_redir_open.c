/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_redir_open.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:27:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:31:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	failed_open_set_error(t_msh *sh, char *filename)
{
	ms_perror(filename);
	sh->exit_code = EXIT_FAILURE;
}

int	open_input_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		failed_open_set_error(sh, filename);
	return (fd);
}

int	open_output_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_PERMISSIONS);
	if (fd == -1)
		failed_open_set_error(sh, filename);
	return (fd);
}

int	open_append_redirection(t_msh *sh, char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, OUTPUT_PERMISSIONS);
	if (fd == -1)
		failed_open_set_error(sh, filename);
	return (fd);
}
