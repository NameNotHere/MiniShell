/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/10 19:48:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Check if character at position is part of a token.
	Returns true if character exists, is not whitespace, and is not an operator.
	EXP_MARK (expansion marker) is treated as a token character.
	Used to identify token boundaries during lexical analysis.
*/
static bool	is_token_char(char *str, int pos)
{
	if (str[pos] == EXP_MARK)
		return (true);
	return (str[pos] && !ft_isspace(str[pos]) && is_operator(str, pos) == 0);
}

void	skip_spaces(int *i, char *str)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\n' || str[*i] == '\t'))
		(*i)++;
}

int	count_tokens(char *str, int count, int i)
{
	char	quote;

	while (str[i])
	{
		skip_spaces(&i, str);
		if (is_operator(str, i) > 0)
			i += is_operator(str, i);
		else if ((str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
			{
				if (str[i] == '\\' && str[i + 1] != '\0')
					i++;
				i++;
			}
			if (str[i] && str[i] == quote)
				i++;
		}
		else if (is_token_char(str, i))
			while (is_token_char(str, i))
				i++;
		count++;
	}
	return (count);
}

void	update_quoted_len(char *str, int *len, int i)
{
	char	quote;
	int		j;

	quote = str[i];
	j = i + 1;
	while (str[j])
	{
		if (str[j] == quote)
		{
			j++;
			break ;
		}
		if (str[j] == '\\' && str[j + 1])
			j += 2;
		else
			j++;
	}
	(*len) += j - i;
}

void	calculate_token_word_len(char *str, int *i, int *len)
{
	skip_spaces(i, str);
	*len = 0;
	while (is_token_char(str, *i + *len))
	{
		if (str[*i + *len]
			&& (str[*i + *len] == '\'' || str[*i + *len] == '\"'))
			update_quoted_len(str, len, *i + *len);
		else
		{
			while (is_token_char(str, *i + *len)
				&& str[*i + *len] != '\'' && str[*i + *len] != '\"')
			{
				if (str[*i + *len] == '\\' && str[*i + *len + 1])
					*len += 2;
				else
					(*len)++;
			}
		}
	}
	if (*len == 0 && str[*i] && is_operator(str, *i))
		*len += is_operator(str, *i);
	else if (*len == 0 && str[*i])
		*len = 1;
}

char	*make_token_word(char *str, int *i, int *err)
{
	int		len;
	char	*word;

	calculate_token_word_len(str, i, &len);
	(*i) += len;
	if (len == 0)
	{
		*err = EXIT_FAILURE;
		return (NULL);
	}
	if (xe_calloc_char(&word, err, len + 1) != EXIT_SUCCESS)
		return (NULL);
	ft_memcpy(word, str + (*i - len), len);
	word[len] = '\0';
	return (word);
}
