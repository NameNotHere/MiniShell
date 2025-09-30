/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/23 16:48:38 by tda-roch         ###   ########.fr       */
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
		if (!str[i])
			break ;
		if (isminioperator(str, i) > 0)
			i += isminioperator(str, i);
		if (!str[i])
			break ;
		else if ((str[i] == '\'' || str[i] == '\"'))
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
			{
				if (str[i] == '\\' && str[i + 1] != '\0')
					i += 2;
				else
					i++;
			}
			if (str[i] && str[i] == quote)
				i++;
			count++;
		}
		else if (str[i] && !ft_isspace(str[i]) && isminioperator(str, i) == 0)
		{
			while (str[i] && !ft_isspace(str[i]) && isminioperator(str, i) == 0)
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
	while (str[i] && !ft_isspace(str[i]) && !isminioperator(str, i) &&\
            str[i] != '\'' && str[i] != '\"')
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
	while (str[*i + len] && !ft_isspace(str[*i + len])
		&& !isminioperator(str, *i + len))
	{
		if (str[*i + len] && (str[*i + len] == '\'' || str[*i + len] == '\"'))
			make_string(str, &len, *i + len);
		else
		{
			while (str[*i + len] && !ft_isspace(str[*i + len])
				&& !isminioperator(str, *i + len)
				&& str[*i + len] != '\'' && str[*i + len] != '\"')
			{
				if (str[*i + len] == '\\' && str[*i + len + 1])
					len += 2;
				else
					len++;
			}
		}
	}
	if (len == 0 && str[*i] && isminioperator(str, *i))
		len += isminioperator(str, *i);
	else if (len == 0 && str[*i])
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
