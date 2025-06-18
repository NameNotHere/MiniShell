/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:53:14 by otanovic          #+#    #+#             */
/*   Updated: 2024/12/01 12:10:19 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static int	word_count(char const *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*get_word(char const *s, char c, int *start)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (s[*start + len] && s[*start + len] != c)
		len++;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[*start + i];
		i++;
	}
	word[i] = '\0';
	*start += len;
	return (word);
}

static char	**free_ar(char **newar)
{
	int	i;

	i = 0;
	while (newar[i] != NULL)
		free(newar[i++]);
	free(newar);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**newar;
	int		wc;
	int		i;
	int		start;

	start = 0;
	i = 0;
	wc = word_count(s, c);
	newar = malloc(sizeof(char *) * (wc + 1));
	if (!newar || !s)
		return (NULL);
	while (s[start] == c)
		start++;
	while (i < wc)
	{
		while (s[start] == c)
			start++;
		newar[i] = get_word(s, c, &start);
		if (!newar[i])
			return (free_ar(newar));
		i++;
	}
	newar[i] = NULL;
	return (newar);
}
