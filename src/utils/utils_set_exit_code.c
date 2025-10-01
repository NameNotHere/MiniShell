/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_set_exit_code.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:52:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 00:31:09 by tda-roch         ###  ########.fr       */
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
	Sets the shell exit code.
	Returns the same value.
*/
int	ret_exit(t_msh *sh, int exit_code)
{
	sh->exit_code = exit_code;
	return (exit_code);
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
	Sets the shell exit code, prints error message.
	Returns the same value.
*/
int	ret_exit_msg(t_msh *sh, int exit_code, const char *error_msg)
{
	sh->exit_code = exit_code;
	msg_err(error_msg);
	return (exit_code);
}

/*
	Sets exit code to errno or specific error.
	Tries to use errno if ERRNO_CODE (-1) value is passed.
		Fallsback to EXIT_FAILURE (1).
	Prints error with perror (or msg_err if no errno).
*/
void	set_exit_perr(t_msh *sh, int exit_code, const char *error_msg)
{
	if (exit_code != ERRNO_CODE)
		sh->exit_code = exit_code;
	else if (exit_code == ERRNO_CODE && !errno)
		sh->exit_code = EXIT_FAILURE;
	else
		sh->exit_code = errno;
	if (errno)
		perror(error_msg);
	else
		msg_err(error_msg);
}

/*
	Sets exit code to errno or specific error.
	Tries to use errno if ERRNO_CODE (-1) value is passed.
		Fallsback to EXIT_FAILURE (1).
	Prints error with perror (or msg_err if no errno).
	Returns the same value previously set.
*/
int	ret_exit_perr(t_msh *sh, int exit_code, const char *error_msg)
{
	if (exit_code != ERRNO_CODE)
		sh->exit_code = exit_code;
	else if (exit_code == ERRNO_CODE && !errno)
		sh->exit_code = EXIT_FAILURE;
	else
		sh->exit_code = errno;
	if (errno)
		perror(error_msg);
	else
		msg_err(error_msg);
	return (sh->exit_code);
}
