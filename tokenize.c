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

#include <minishell.h>

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

char	*make_word(char *str)
{
	int		i;
	int		s;
	char	*word;

	i = 0;
	s = 0;
	while (*str && (str[s] != ' ' || str[s] != '\n' || str[s] != '\t'\
		|| str[s] != '\'' || str[s] != '\"'))
		s++;
	word = malloc(i * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i <= s)
		word[i] = str[i++];
	word[s] = '\0';
	return (word);
}

t_token	token(char *str)
{
	t_token	output;

	output.word = str;
	if (strcmp(str, "<<"))
		output.ty = TOKEN_HERODOC;
	else if (strcmp(str, ">>"))
		output.ty = TOKEN_APPEND;
	else if (strcmp(str, "<"))
		output.ty = TOKEN_OUTPUT;
	else if (strcmp(str, ">"))
		output.ty = TOKEN_INPUT;
	else if (strcmp(str, "\""))
		output.ty = DOUBLE_QUOTE;
	else if (strcmp(str, "\'"))
		output.ty = SINGLE_QUOTE;
	else if (str[0] == '-')
		output.ty = TOKEN_PARAMETER;
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
	int		t_size;
	int		t_amount;
	char	*token;

	t_amount = count_tokens(input);
	i = 0;
	output = malloc(sizeof(t_token) * t_amount);
		return (NULL);
	while (t_amount--)
	{
		token = make_word(&input[i]); // might also not work
		i += sizeof(token); // might not work
		output[t_amount] = token(token);
		while (input[i] == '\n' || input[i] == ' ' || input[i] == '\t')
			i++;
	}
	return (output);
}
