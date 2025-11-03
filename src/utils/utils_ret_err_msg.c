/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ret_err_msg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 09:57:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/03 20:35:11 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	prints error message, frees a string, returns a value.
	the three parameters are passed in this order.
*/
int	ret_msg_free_str(const char *error, char **to_free, int ret)
{
	msg_err(error);
	safe_free_str(to_free);
	return (ret);
}
