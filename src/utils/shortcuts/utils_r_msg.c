/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_r_msg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:55:01 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 12:43:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Prints error message and returns a value.
*/
int	r_msg_err(const char *error_msg, int ret)
{
	msg_err(error_msg);
	return (ret);
}

int	r_msg_two_err(const char *str1, const char *str2, int ret)
{
	msg_err_2(str1, str2);
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
