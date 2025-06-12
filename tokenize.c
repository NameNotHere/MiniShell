/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/12 16:02:44 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int count_tokens(char *str)
{
	int count = 0;
	int i = 0;

	while (str[i])
	{
		skip_spaces(&i, str);
		if (str[i])
		{
			count++;

			// Skip over the current token
			if (str[i] == '\'' || str[i] == '\"')
			{
				char quote = str[i++];
				while (str[i] && str[i] != quote)
					i++;
				if (str[i] == quote)
					i++;
			}
			else
			{
				while (str[i] && ((str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123)))
					i++;
			}
		}
	}
	return count;
}

char *make_word(char *str, int *i)
{
	int start;
	int len = 0;
	char *word;
	char quote;

	skip_spaces(i, str);

	if (!str[*i])
		return NULL;

	start = *i;

	if (str[*i] == '\'' || str[*i] == '\"')
	{
		quote = str[*i];
		start = *i;
		(*i)++;
		while (str[*i] && str[*i] != quote)
		{
			(*i)++;
			len++;
		}
		if (str[*i] == quote)
			(*i)++;
	}
	else
	{
		while (str[*i] && ((str[*i] > 64 && str[*i] < 91) || (str[*i] > 96 && str[*i] < 123)))
		{
			(*i)++;
			len++;
		}
	}
	word = malloc(len + 1);
	if (!word)
		return NULL;
	for (int j = 0; j < len; j++) // remove
		word[j] = str[start + j];
	word[len] = '\0';
	return word;
}


// this doesnt work
t_token	ft_token(char *str)
{
	t_token	output;

	output.word = str;
	if (ft_strcmp(str, "<<") == 0)
		output.ty = TOKEN_HEREDOC;
	else if (ft_strcmp(str, ">>") == 0)
		output.ty = TOKEN_APPEND;
	else if (ft_strcmp(str, "<") == 0)
		output.ty = TOKEN_OUTPUT;
	else if (ft_strcmp(str, ">") == 0)
		output.ty = TOKEN_INPUT;
	else if (ft_strcmp(str, "\"") == 0)
		output.ty = TOKEN_DOUBLE_QUOTE;
	else if (ft_strcmp(str, "\'") == 0)
		output.ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-')
		output.ty = TOKEN_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else
		output.ty = TOKEN_COMMAND;
	return (output);
}

t_token	*tokenize(char *input, int *token_count)
{
	t_token	*output;
	int		i;
	int		t_amount;
	char	*token;
	int		id;

	t_amount = count_tokens(input);
	i = 0;
	output = malloc(sizeof(t_token) * t_amount);
	if (!output)
		return (NULL);
	id = 0;
	while (t_amount--)
	{
		skip_spaces(&i, input);
		token = make_word(input, &i);
		if (token)
			output[id++] = ft_token(token);
		else
		{
			free(output);
			return (NULL);
		}
	}
	*token_count = id;
	return (output);
}
