/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parser_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 06:59:52 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 07:46:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *str)
{
	if (!str)
		return (false);
	if (ft_strcmp(str, "cd") == 0)
		return (true);
	else if (ft_strcmp(str, "echo") == 0)
		return (true);
	else if (ft_strcmp(str, "pwd") == 0)
		return (true);
	else if (ft_strcmp(str, "export") == 0)
		return (true);
	else if (ft_strcmp(str, "unset") == 0)
		return (true);
	else if (ft_strcmp(str, "env") == 0)
		return (true);
	else if (ft_strcmp(str, "exit") == 0)
		return (true);
	return (false);
}

/*
	Returns true if found an escaped char.

	Note: to determine if scaped, it needs to check that not only the previous
	char is a backslash, but also that if there are more backslashes, the total
	number is an odd number. otherwise we just had a series of literal
	backslashes(escaped backslashes, no actual escape char).
	only checked if PRO (otherwise automatically false)
*/
bool	is_escaped(const char *str, int i)
{
	int	backslash_count;
	int	j;

	if (!PRO)
		return (false);
	backslash_count = 0;
	if (i <= 0)
		return (false);
	j = i - 1;
	while (j >= 0 && str[j] == '\\')
	{
		backslash_count++;
		j--;
	}
	return (backslash_count % 2);
}

bool	has_quotes(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (true);
		str++;
	}
	return (false);
}
