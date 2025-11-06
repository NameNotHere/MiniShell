/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_r_plus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:17:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 14:47:06 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	frees a string, returns a value.
*/
int	r_free_str(char **to_free, int ret)
{
	safe_free_str(to_free);
	return (ret);
}

/*
	frees two strings, returns a value.
*/
int	r_free_two_str(char **str_a, char **str_b, int ret)
{
	safe_free_str(str_a);
	safe_free_str(str_b);
	return (ret);
}

/*
	frees a string, sets shell exit code to EXIT_FAILURE with system error message.
	Returns EXIT_FAILURE.
*/
int	r_free_str_perr(t_msh *sh, char **to_free, const char *error_msg)
{
	safe_free_str(to_free);
	return (r_set_exit_perr(sh, error_msg));
}
