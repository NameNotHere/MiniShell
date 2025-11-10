/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_r_set_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 09:58:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 19:48:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Sets the shell exit code and returns the same value.
*/
int	r_set_exit(t_msh *sh, int exit_code)
{
	sh->exit_code = exit_code;
	return (exit_code);
}

/*
	Sets the shell exit code, prints error message, and returns the value.
*/
int	r_set_exit_msg(t_msh *sh, int exit_code, const char *error_msg)
{
	sh->exit_code = exit_code;
	msg_err(error_msg);
	return (exit_code);
}



/*
	Sets exit code to EXIT_FAILURE and prints system error message.
	Error code is always EXIT_FAILURE, not errno.
	Returns EXIT_FAILURE.
*/
int	r_set_exit_perr(t_msh *sh, const char *error_msg)
{
	sh->exit_code = EXIT_FAILURE;
	msg_perr(error_msg);
	return (EXIT_FAILURE);
}

/*
	Sets the shell exit code and returns a different value.
	Useful for setting exit code while returning a different return value
	(e.g., enum).
*/
int	r_set_exit_ret(t_msh *sh, int exit_code, int ret)
{
	sh->exit_code = exit_code;
	return (ret);
}

/*
	Prints error message and returns a value.
*/
int	r_msg_err(const char *error_msg, int ret)
{
	msg_err(error_msg);
	return (ret);
}

/*
	Prints system error message and returns a value.
*/
int	r_msg_perr(const char *error_msg, int ret)
{
	msg_perr(error_msg);
	return (ret);
}

/*
	Frees all shell resources and returns a value.
*/
int	r_free_everything(t_msh *sh, int ret)
{
	free_everything(sh);
	return (ret);
}
