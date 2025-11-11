/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 11:23:20 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/30 11:23:25 by tda-roch         ###   ########.fr       */
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

long long	ft_atoll(const char *s)
{
	int			i;
	long long	num;
	int			sign;

	i = 0;
	num = 0;
	sign = 1;
	i = skip_initial_spaces(s, i);
	handle_sign(s, &i, &sign);
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (num > (LLONG_MAX - (s[i] - '0')) / 10)
		{
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		num = num * 10 + (s[i] - '0');
		i++;
	}
	return (num * sign);
}
