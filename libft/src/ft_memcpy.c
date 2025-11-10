/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:58:24 by otanovic          #+#    #+#             */
/*   Updated: 2024/12/01 12:10:09 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t num)
{
	const unsigned char	*s;
	unsigned char		*ptr;
	size_t				i;

	if (dst == NULL && src == NULL && num != 0)
		return ((unsigned char *) dst);
	i = 0;
	s = (const unsigned char *)src;
	ptr = (unsigned char *) dst;
	while (i < num)
	{
		ptr[i] = s[i];
		i++;
	}
	return (ptr);
}
