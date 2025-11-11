/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_escaped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:57:27 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
