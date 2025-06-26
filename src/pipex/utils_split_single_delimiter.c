/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split_single_delimiter.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 18:52:06 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 12:04:44 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_numstrings(char const *s, char const c)
{
	int	numstrings;
	int	i;

	numstrings = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		numstrings++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (numstrings);
}

static char	*get_next_str(int *str_i, char const *s, char const c)
{
	int		slen;
	int		spos;
	int		i;
	char	*subs;

	while (s[*str_i] != '\0' && s[*str_i] == c)
		(*str_i)++;
	spos = *str_i;
	slen = 0;
	while (s[*str_i] != '\0' && s[*str_i] != c)
	{
		slen++;
		(*str_i)++;
	}
	subs = ft_calloc((slen + 1), sizeof(char));
	if (!subs)
		return (NULL);
	i = 0;
	while (s[spos] != '\0' && s[spos] != c)
		subs[i++] = s[spos++];
	subs[i] = '\0';
	return (subs);
}

/*
Splits a string using a single delimiter character.

Parameters:
	s: The string to be split
	c: delimiter char
Return value:
	Returns an array of strings, each string being a substring of s
		separated by delimiter char(s).
Description:
	Allocates memory and returns an array of strings obtained by
	splitting ’s’ using the character ’c’ as a delimiter. The array must end
	with a NULL pointer.
	Returns NULL if allocation fails/
*/
char	**split_single_delimiter(char const *s, const char c)
{
	char	**result;
	int		numstrs;
	int		str_count;
	int		str_i;

	str_count = 0;
	str_i = 0;
	numstrs = get_numstrings(s, c);
	result = ft_calloc((numstrs + 1), sizeof(char *));
	if (!result)
		return (NULL);
	while (str_count < numstrs)
	{
		result[str_count] = get_next_str(&str_i, s, c);
		if (!result[str_count])
		{
			safe_free_2d_string(&result);
			return (NULL);
		}
		str_count++;
	}
	result[str_count] = NULL;
	return (result);
}
