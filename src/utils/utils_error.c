/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:04:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 00:57:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	msg_err(const char *error)
{
	while (*error)
		write(STDERR_FILENO, error++, 1);
}

void	msg_err_2(const char *str1, const char *str2)
{
	msg_err(str1);
	msg_err(str2);
}

void	msg_err_3(const char *str1, const char *str2, const char *str3)
{
	msg_err(str1);
	msg_err(str2);
	msg_err(str3);
}
