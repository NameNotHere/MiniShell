/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:04:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:32:03 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

void	msg_err(const char *str1)
{
	write(STDERR_FILENO, E_MINISHELL, sizeof(E_MINISHELL));
	while (*str1)
		write(STDERR_FILENO, str1++, 1);
	write(STDERR_FILENO, "\n", 1);
}

void	msg_err_2(const char *str1, const char *str2)
{
	write(STDERR_FILENO, E_MINISHELL, sizeof(E_MINISHELL));
	while (*str1)
		write(STDERR_FILENO, str1++, 1);
	while (*str2)
		write(STDERR_FILENO, str2++, 1);
	write(STDERR_FILENO, "\n", 1);
}

void	msg_err_3(const char *str1, const char *str2, const char *str3)
{
	write(STDERR_FILENO, E_MINISHELL, sizeof(E_MINISHELL));
	while (*str1)
		write(STDERR_FILENO, str1++, 1);
	while (*str2)
		write(STDERR_FILENO, str2++, 1);
	while (*str3)
		write(STDERR_FILENO, str3++, 1);
	write(STDERR_FILENO, "\n", 1);
}

int	msg_err_and_free_string(const char *str1, char **to_free)
{
	msg_err(str1);
	safe_free_string(to_free);
	return (EXIT_FAILURE);
}

