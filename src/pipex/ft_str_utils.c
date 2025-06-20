/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 11:09:24 by tda-roch          #+#    #+#             */
/*   Updated: 2025/04/14 21:11:39 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

/*
Returns a pointer to a new string which is a duplicate of the string src.
Memory for the new string is obtained with malloc, and can be freed with free.
*/
char	*ft_strdup(char const *src)
{
	char	*new_src;
	size_t	i;

	i = 0;
	new_src = malloc((ft_strlen(src) + 1) * sizeof(char));
	if (!new_src)
		return (NULL);
	while (src[i] != '\0')
	{
		new_src[i] = src[i];
		i++;
	}
	new_src[i] = '\0';
	return (new_src);
}

/*
counts and returns the number of characters in a string
*/
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	c1;
	unsigned char	c2;

	if (n == 0)
		return (0);
	while (n-- > 0)
	{
		c1 = (unsigned char)*s1;
		c2 = (unsigned char)*s2;
		if (c1 != c2)
			return (c1 - c2);
		if (c1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (0);
}

/*
Allocates (with malloc) and returns a new
string, which is the result of the concatenation
of ’s1’ and ’s2’.
Parameters:
	s1: The prefix string.
	s2: The suffix string.
Return value:
	The new string.
	NULL if the allocation fails.
*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc((len1 + len2 + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len1)
		result[i++] = *s1++;
	while (i - len1 < len2)
		result[i++] = *s2++;
	result[i] = '\0';
	return (result);
}
