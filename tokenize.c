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

t_token	ft_token(char *str)
{
	t_token	output;

	output.word = str;
	if (ft_strncmp(str, "<<", 2) == 0)
		output.ty = TOKEN_HEREDOC;
	else if (ft_strncmp(str, ">>", 2) == 0)
		output.ty = TOKEN_APPEND;
	else if (ft_strncmp(str, "<", 1) == 0)
		output.ty = TOKEN_OUTPUT;
	else if (ft_strncmp(str, ">", 1) == 0)
		output.ty = TOKEN_INPUT;
	else if (ft_strncmp(str, "\"", 1) == 0)
		output.ty = TOKEN_DOUBLE_QUOTE;
	else if (ft_strncmp(str, "\'", 1) == 0)
		output.ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-') // this might be wrong as params can be without - ?
		output.ty = TOKEN_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else if (str[0] == '|')
		output.ty = TOKEN_PIPE;
	else if (str[0] == '.' || str[0] == '/' || str[0] == '~')
		output.ty = TOKEN_FILE_PATH;
	else if (ft_isdigit(str[0]))
		output.ty = TOKEN_NUMBER;
	else if (is_builtin(str) == 1)
		output.ty = TOKEN_INBUILT;
	else
		output.ty = TOKEN_WORD;
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
