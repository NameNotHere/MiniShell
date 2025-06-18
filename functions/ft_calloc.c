/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:56:45 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/18 12:42:04 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>
#include "minishell.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*c;
	size_t			i;

	if (size == 0)
		return (malloc(0));
	if (SIZE_MAX / size < nmemb)
		return (NULL);
	i = 0;
	c = (unsigned char *)malloc(nmemb * size);
	if (c == NULL)
		return (NULL);
	while (i < nmemb * size)
	{
		c[i] = 0;
		i++;
	}
	return ((void *)c);
}
