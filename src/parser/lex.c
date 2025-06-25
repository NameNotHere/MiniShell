/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/25 12:49:46 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	skip_spaces(int *i, char *str)
{
	int	y;

	y = 0;
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\n' || str[*i] == '\t'))
	{
		(*i)++;
		y++;
	}
	return (y);
}

int	count_tokens(char *str)
{
	int		count;
	int		i;
	char	quote;

	i = 0;
	count = 0;
	while (str[i])
	{
		skip_spaces(&i, str);
		if (str[i] && str[i +1] && isminioperator(str, i) > 0)
		{
			count++;
			i += isminioperator(str, i);
		}
		else if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i++];
			is_closed(str, i, quote);
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				i++;
			count++;
		}
		else
		{
			while (str[i] && !ft_isspace(str[i]) && str[i] != '\'' \
				&& str[i] != '\"' && isminioperator(str, i) == 0)
				i++;
			count++;
		}
	}
	return (count);
}

void	make_string(char *str, int *len, int i)
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

void	parse_word(char *str, int i, int *len)
{
	if (str[i] == '-')
	{
		(*len)++;
		i++;
	}
	while (str[i] && !ft_isspace(str[i]) && str[i] != '\'' && \
		str[i] != '\"' && !isminioperator(str, i))
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			*len += 2;
		}
		else
		{
			i++;
			(*len)++;
		}
	}
}

char	*make_word(char *str, int *i, int *err)
{
	int		len;
	char	*word;

	skip_spaces(i, str);
	len = 0;
	if (str[*i] && (str[*i] == '\'' || str[*i] == '\"'))
		make_string(str, &len, *i);
	else if (str[*i] && (ft_isalpha(str[*i]) || ft_strchr(".$~*-", str[*i])
			|| ft_isdigit(str[*i + len])))
		parse_word(str, *i, &len);
	else if (str[*i] && isminioperator(str, *i))
		len += isminioperator(str, *i);
	else if (str[*i])
		len = 1;
	(*i) += len;
	if (len == 0)
		*err = EXIT_FAILURE;
	if (len == 0)
		return (NULL);
	*err = mallo_x((void **)&word, (len + 1), sizeof(char));
	if (*err)
		return (NULL);
	ft_memcpy(word, str + (*i - len), len);
	word[len] = '\0';
	return (word);
}
