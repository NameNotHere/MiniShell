/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ret_plus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:17:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/03 11:49:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	frees a string, returns a value.
	the two parameters are passed in this order.
*/
int	ret_free_string(char **to_free, int ret)
{
	safe_free_string(to_free);
	return (ret);
}
