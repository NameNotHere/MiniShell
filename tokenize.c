/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/09 15:06:41 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
			i++;
		if (str[i])
		{
			count++;
			while (str[i] && str[i] != ' ' && str[i] != '\n' && str[i] != '\t')
				i++;
		}
	}
	return (count);
}

#include <stdlib.h>

char	*make_word(char *str, int *i)
{
	int		start;
	int		len;
	char	*word;
	int		j;

	start = *i;
	len = 0;

	while (str[*i] && str[*i] != ' ' && str[*i] != '\n' &&
		str[*i] != '\t' && str[*i] != '\'' && str[*i] != '\"')
	{
		(*i)++;
		len++;
	}
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (j < len)
	{
		word[j] = str[start + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}


t_token	ft_token(char *str)
{
	t_token	output;

	output.word = str;
	if (ft_strcmp(str, "<<"))
		output.ty = TOKEN_HEREDOC;
	else if (ft_strcmp(str, ">>"))
		output.ty = TOKEN_APPEND;
	else if (ft_strcmp(str, "<"))
		output.ty = TOKEN_OUTPUT;
	else if (ft_strcmp(str, ">"))
		output.ty = TOKEN_INPUT;
	else if (ft_strcmp(str, "\""))
		output.ty = TOKEN_DOUBLE_QUOTE;
	else if (ft_strcmp(str, "\'"))
		output.ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-')
		output.ty = TOKEN_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else
		output.ty = TOKEN_WORD;
	return (output);
}

t_token	*tokenize(char *input)
{
	t_token	*output;
	int		i;
	int		t_amount;
	char	*token;

	t_amount = count_tokens(input);
	i = 0;
	output = malloc(sizeof(t_token) * t_amount);
	if (!output)
		return (NULL);
	while (t_amount--)
	{
		token = make_word(input, &i); // might also not work
		i += sizeof(token) + 1; // might not work
		output[t_amount] = ft_token(token);
		while (input[i] == '\n' || input[i] == ' ' || input[i] == '\t')
			i++;
	}
	return (output);
}
