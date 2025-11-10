/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_r_plus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:17:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 19:48:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	frees a string, returns NULL.
*/
void	*r_free_str_null(char **to_free)
{
	safe_free_str(to_free);
	return (NULL);
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
	frees a string, sets shell exit code to EXIT_FAILURE with system error
	message.
	Returns EXIT_FAILURE.
*/
int	r_free_str_perr(t_msh *sh, char **to_free, const char *error_msg)
{
	safe_free_str(to_free);
	return (r_set_exit_perr(sh, error_msg));
}

/*
	frees a pointer, sets it to NULL, returns NULL.
*/
void	*r_free_null(void **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (NULL);
}
