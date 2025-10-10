/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_escaped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/01 02:13:46 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_escaped(const char *str, int i)
{
	int	backslash_count;
	int	j;
	int	ret;

	backslash_count = 0;
	if (i <= 0)
		return (false);
	j = i - 1;
	while (j >= 0 && str[j] == '\\')
	{
		backslash_count++;
		j--;
	}
	ret = backslash_count % 2;
	if (ret != 0)
		return (true);
	else
		return (false);
}
