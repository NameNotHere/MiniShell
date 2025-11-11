/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:15:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Check if character at position is part of a token.
	Returns true if character exists, is not whitespace, and is not an operator.
	EXP_MARK (expansion marker) is treated as a token character.
	Used to identify token boundaries during lexical analysis.
*/
bool	is_token_char(char *str, int pos)
{
	if (str[pos] == EXP_MARK)
		return (true);
	return (str[pos] && !ft_isspace(str[pos]) && is_operator(str, pos) == 0);
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
