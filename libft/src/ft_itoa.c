/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 18:55:11 by otanovic          #+#    #+#             */
/*   Updated: 2024/12/01 12:09:48 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include "libft.h"

static int	get_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

void	checkneg(int *num, char *str)
{
	if (*num == INT_MIN)
	{
		str[0] = '-';
		str[1] = '2';
		*num = 147483648;
	}
	else
	{
		str[0] = '-';
		*num = -*num;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		num;
	int		*p;

	num = n;
	p = &num;
	len = get_num_len(num);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num < 0)
	{
		checkneg(p, str);
	}
	while (num > 0)
	{
		str[--len] = (num % 10) + '0';
		num /= 10;
	}
	if (n == 0)
		str[0] = '0';
	return (str);
}
