/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:04:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/14 03:20:15 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	put_stderr(const char *error)
{
	while (*error)
		write(STDERR_FILENO, error++, 1);
}

void	put_stderr_2(const char *str1, const char *str2)
{
	put_stderr(str1);
	put_stderr(str2);
}

void	put_stderr_3(const char *str1, const char *str2, const char *str3)
{
	put_stderr(str1);
	put_stderr(str2);
	put_stderr(str3);
}
