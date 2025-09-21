/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:57:06 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/21 02:24:13 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strdup(const char *str)
{
	int		size;
	char	*dup;

	if (!str)
		return (NULL);
	size = 0;
	while (str[size] != '\0')
		size++;
	dup = malloc(sizeof(char) * (size + 1));
	if (dup == NULL)
		return (NULL);
	size = 0;
	while (str[size] != '\0')
	{
		dup[size] = str[size];
		size++;
	}
	dup[size] = '\0';
	return (dup);
}
