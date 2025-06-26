/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 18:52:06 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 12:04:44 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pipex.h"

int		get_numstrings_q(char *str, char *charset);
char	*getnextstr_q(size_t *str_i, char *str, char *charset, char quote);

/*
Splits a string using a character set of delimiters and quotes.

Parameters:
	str: The string to be split
	charset: string containing delimiter chars
Return value:
	Returns an array of strings, each string being a substring of str
		separated by any char(s) in charset or by quotes.
Description:
	Allocates memory for the array of strings and each substring.
	Each substring is a copy of the corresponding part of str.
	Each string in the array is null-terminated as well as the array itself.
	Empty strings are skipped except if inside quotes.
	Delimiter chars inside quotes are not removed.
*/
char	**split_charset_using_quote(char *str, char *charset)
{
	char	**result;
	size_t	numstrs;
	size_t	str_count;
	size_t	str_i;

	if (!str || !charset)
		return (NULL);
	str_count = 0;
	str_i = 0;
	numstrs = get_numstrings_q(str, charset);
	result = ft_calloc((numstrs + 1), sizeof(char *));
	if (result == NULL)
		return (NULL);
	while (str_count < numstrs)
	{
		result[str_count] = getnextstr_q(&str_i, str, charset, 0);
		if (result[str_count] == NULL)
		{
			safe_free_2d_string(&result);
			return (NULL);
		}
		str_count++;
	}
	result[str_count] = NULL;
	return (result);
}

/*
Counts substrings in a string separated by any char(s) in charset,
keeping quoted strings together.

Parameters:
	str: The string to be analyzed
	charset: string containing delimiter chars

Return value:
	Number of substrings counted.

Description:
	Counts substrings from string, but skips consecutive
	delimiter chars and only counts non-empty substrings.
	Empty quotes are preserved as empty strings.
	Spaces within quotes are preserved.
	Handles both single and double quotes.
*/
int	get_numstrings_q(char *str, char *charset)
{
	size_t	numstrings;
	size_t	i;
	char	active_quote;

	numstrings = 0;
	active_quote = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && c_in_str(str[i], charset))
			i++;
		if (str[i] == '\0')
			break ;
		numstrings++;
		while (str[i] != '\0' && (!c_in_str(str[i], charset)
				|| active_quote != 0))
		{
			if (c_in_str(str[i], QUOTES) && active_quote == 0)
				active_quote = str[i];
			else if (c_in_str(str[i], QUOTES) && active_quote == str[i])
				active_quote = 0;
			i++;
		}
	}
	return (numstrings);
}

void	set_next_string_empty_quotes(t_nxtsq *nx, size_t *str_i)
{
	nx->empty_quotes = true;
	*str_i += 2;
	nx->end_pos = *str_i;
}

void	parse_nxtst_q(t_nxtsq *nx, size_t *str_i, char *str, char *charset)
{
	char	quote;

	while (str[*str_i] != '\0' && c_in_str(str[*str_i], charset))
		(*str_i)++;
	nx->str_pos = *str_i;
	if (c_in_str(str[*str_i], QUOTES) && str[*str_i] == str[*str_i + 1])
		return (set_next_string_empty_quotes(nx, str_i));
	quote = 0;
	while (str[*str_i] != '\0')
	{
		if (c_in_str(str[*str_i], QUOTES))
		{
			if (quote == 0)
				quote = str[*str_i];
			else if (quote == str[*str_i])
				quote = 0;
			(*str_i)++;
			continue ;
		}
		if (quote == 0 && c_in_str(str[*str_i], charset))
			break ;
		nx->str_len++;
		(*str_i)++;
	}
	nx->end_pos = *str_i;
}

char	*getnextstr_q(size_t *str_i, char *str, char *charset, char quote)
{
	t_nxtsq	nx;
	char	*substr;

	ft_bzero(&nx, sizeof(nx));
	parse_nxtst_q(&nx, str_i, str, charset);
	if (nx.empty_quotes)
		return (get_empty_string());
	substr = ft_calloc((nx.str_len + 1), sizeof(char));
	if (substr == NULL)
		return (NULL);
	while (nx.str_pos < nx.end_pos)
	{
		if (c_in_str(str[nx.str_pos], QUOTES))
		{
			if (quote == 0)
				quote = str[nx.str_pos];
			else if (quote == str[nx.str_pos])
				quote = 0;
			nx.str_pos++;
		}
		else
			substr[nx.write_pos++] = str[nx.str_pos++];
	}
	substr[nx.write_pos] = '\0';
	return (substr);
}
