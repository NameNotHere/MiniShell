/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:52:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/25 05:25:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// temp_print cannot be turned off
// this is for specific & temporary tests
// this function call will be removed from code
void	temp_print(const char *str, ...)
{
	va_list	args;

	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
}
