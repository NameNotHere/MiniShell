/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_r_err_msg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:57:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 12:49:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	prints error message, frees a string, returns a value.
	the three parameters are passed in this order.
*/
int	r_msg_err_free_str(const char *error, char **to_free, int ret)
{
	msg_err(error);
	safe_free_str(to_free);
	return (ret);
}

/*
	prints error message,
	returns NULL
*/
void	*msg_err_null(const char *error)
{
	msg_err(error);
	return (NULL);
}

/*
	prints error message with errno,
	returns NULL
*/
// TODO: check if this function is being used.
void	*msg_perr_null(const char *error)
{
	msg_perr(error);
	return (NULL);
}
