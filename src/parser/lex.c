/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/10/28 00:13:07 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include "minishell.h"

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
		else if (str[i] && !ft_isspace(str[i]) && is_operator(str, i) == 0)
			while (str[i] && !ft_isspace(str[i]) && is_operator(str, i) == 0)
				i++;
		count++;
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

void	parse_word(char *str, int *i, int *len)
{

	skip_spaces(i, str);
	*len = 0;
	while (str[*i + *len] && !ft_isspace(str[*i + *len])
		&& !is_operator(str, *i + *len))
	{
		if (str[*i + *len] && (str[*i + *len] == '\'' || str[*i + *len] == '\"'))
			make_string(str, len, *i + *len);
		else
		{
			while (str[*i + *len] && !ft_isspace(str[*i + *len])
				&& !is_operator(str, *i + *len)
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

char	*make_word(char *str, int *i, int *err)
{
	int		len;
	char	*word;

	parse_word(str, i, &len);
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
