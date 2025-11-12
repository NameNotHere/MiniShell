/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_set_exit_code.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:52:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 12:43:21 by tda-roch         ###  ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Sets exit code to EXIT_FAILURE, prints system error message.
	Uses msg_perr to print errno-based error info.
*/
void	set_exit_perr(t_msh *sh, const char *error_msg)
{
	sh->exit_code = EXIT_FAILURE;
	msg_perr(error_msg);
}

/*
	Calls set_exit_perr:
		Sets exit code to EXIT_FAILURE,
		prints error message with errno information.
	Returns NULL
*/
void	*r_set_exit_perr_null(t_msh *sh, const char *error_msg)
{
	sh->exit_code = EXIT_FAILURE;
	msg_perr(error_msg);
	return (NULL);
}
