/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/12 14:31:06 by otanovic         ###   ########.fr       */
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

int	count_tokens(char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	printf("y\n");
	while (str[i])
	{// doesnt work
		skip_spaces(&i, str);
		if (str[i])
			count++;
		i++;
	}
	return (count);
}

char	*make_word(char *str, int *i)
{
	int		start;
	int		len;
	char	*word;
	int		j;
	char	quote;

	skip_spaces(i, str);
	len = 0;
	j = 0;
	start = *i;
	while (str[(*i) + len] && (str[(*i) + len] != ' ' && str[(*i) + len] != '\n' && str[(*i) + len] != '\t'))
		len++;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		quote = str[*i];
		(*i)++;
		while (str[*i] && str[*i] != quote)
		{
			(*i)++;
			len++;
		}
	}
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	while (j < len)
	{
		word[j] = str[start + j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

// this doesnt work
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
		output.ty = TOKEN_COMMAND;
	return (output);
}

t_token	*tokenize(char *input)
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
		token = make_word(input, &i); // might also not work
		output[id++] = ft_token(token);
        if (token)
            output[id++] = ft_token(token);
        else
        {
            free(output);
            return (NULL);
        }
	}
	return (output);
}
