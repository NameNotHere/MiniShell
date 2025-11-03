/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_ret_plus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 11:17:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/03 23:14:05 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

/*
	frees a string, returns a value.
*/
int	ret_free_str(char **to_free, int ret)
{
	safe_free_str(to_free);
	return (ret);
}

/*
	frees two strings, returns a value.
*/
int	ret_free_two_str(char **str_a, char **str_b, int ret)
{
	safe_free_str(str_a);
	safe_free_str(str_b);
	return (ret);
}
