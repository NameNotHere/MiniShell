/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:42:20 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/29 13:53:01 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin3(char const *s1, char const *s2, char const *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	size_t	i;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc((len1 + len2 + len3 + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len1)
		result[i++] = *s1++;
	while (i - len1 < len2)
		result[i++] = *s2++;
	while (i - len1 - len2 < len3)
		result[i++] = *s3++;
	result[i] = '\0';
	return (result);
}

