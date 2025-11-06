/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_set_exit_code.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:52:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 14:47:06 by tda-roch         ###  ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Sets the shell exit code.
*/
void	set_exit_code(t_msh *sh, int exit_code)
{
	sh->exit_code = exit_code;
}

/*
	Sets the shell exit code, prints error message.
*/
void	set_exit_msg(t_msh *sh, int exit_code, const char *error_msg)
{
	sh->exit_code = exit_code;
	msg_err(error_msg);
}

/*
	Sets exit code to EXIT_FAILURE, prints system error message.
	Uses ms_perror to print errno-based error info.
*/
void	set_exit_perr(t_msh *sh, const char *error_msg)
{
	sh->exit_code = EXIT_FAILURE;
	ms_perror(error_msg);
}
