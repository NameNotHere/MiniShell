/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:04:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/09 12:22:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "minishell.h"

void	msg_err(const char *str1)
{
	char	buf[4096];
	size_t	len;

	len = 0;
	ft_memcpy(buf, E_MINISHELL, sizeof(E_MINISHELL) - 1);
	len += sizeof(E_MINISHELL) - 1;
	while (*str1 && len < sizeof(buf) - 2)
		buf[len++] = *str1++;
	if (len < sizeof(buf) - 1)
		buf[len++] = '\n';
	write(STDERR_FILENO, buf, len);
}

void	msg_err_2(const char *str1, const char *str2)
{
	char	buf[4096];
	size_t	len;

	len = 0;
	ft_memcpy(buf, E_MINISHELL, sizeof(E_MINISHELL) - 1);
	len += sizeof(E_MINISHELL) - 1;
	while (*str1 && len < sizeof(buf) - 2)
		buf[len++] = *str1++;
	while (*str2 && len < sizeof(buf) - 2)
		buf[len++] = *str2++;
	if (len < sizeof(buf) - 1)
		buf[len++] = '\n';
	write(STDERR_FILENO, buf, len);
}

void	msg_err_3(const char *str1, const char *str2, const char *str3)
{
	char	buf[4096];
	size_t	len;

	len = 0;
	ft_memcpy(buf, E_MINISHELL, sizeof(E_MINISHELL) - 1);
	len += sizeof(E_MINISHELL) - 1;
	while (*str1 && len < sizeof(buf) - 2)
		buf[len++] = *str1++;
	while (*str2 && len < sizeof(buf) - 2)
		buf[len++] = *str2++;
	while (*str3 && len < sizeof(buf) - 2)
		buf[len++] = *str3++;
	if (len < sizeof(buf) - 1)
		buf[len++] = '\n';
	write(STDERR_FILENO, buf, len);
}

