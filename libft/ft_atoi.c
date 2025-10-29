/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:56:38 by otanovic          #+#    #+#             */
/*   Updated: 2025/10/29 18:49:28 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "libft.h"

static int	skip_initial_spaces(const char *s, int i)
{
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\v'
		|| s[i] == '\f' || s[i] == '\r')
		i++;
	return (i);
}

static void	handle_sign(const char *s, int *i, int *sign)
{
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	ft_atoi(const char *s)
{
	int	i;
	int	num;
	int	sign;

	i = 0;
	num = 0;
	sign = 1;
	i = skip_initial_spaces(s, i);
	handle_sign(s, &i, &sign);
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (num > (INT_MAX - (s[i] - '0')) / 10)
		{
			if (sign == 1)
				return (INT_MAX);
			else
				return (INT_MIN);
		}
		num = num * 10 + (s[i] - '0');
		i++;
	}
	return (num * sign);
}
