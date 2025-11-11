/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
