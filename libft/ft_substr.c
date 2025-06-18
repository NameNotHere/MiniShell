/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:57:33 by otanovic          #+#    #+#             */
/*   Updated: 2024/12/01 12:10:37 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t			length;
	size_t			i;
	char			*ret;

	length = (size_t) ft_strlen(s);
	if (start >= length)
		return (ft_strdup(""));
	if (start + len > length)
		len = length - start;
	ret = malloc((char)len + 1);
	if (ret == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[i + start] != '\0')
	{
		ret[i] = s[i + start];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
