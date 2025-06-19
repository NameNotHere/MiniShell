/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/19 02:51:41 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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
// I AGREE WITH YOUR COMMENT BELOW! Although, token may be useful to keep because some built ins are explicitly not accepting dash params.
// SUGGESTION: call it DASH_PARAM?
	else if (str[0] == '-') // this might be wrong as params can be without - ?
		output.ty = TOKEN_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else if (ft_strncmp(str, "&&", 2) == 0)
		output.ty = TOKEN_AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		output.ty = TOKEN_OR;
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
	return (output); // mqybe remove the output definition
}

const char	*get_token_name(t_token_type type)
{
	if (type < 0 || type > TOKEN_OR)
		return ("UNKNOWN");
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_INBUILT)
		return ("INBUILT");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_INPUT)
		return ("INPUT");
	if (type == TOKEN_OUTPUT)
		return ("OUTPUT");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (type == TOKEN_SINGLE_QUOTE)
		return ("SINGLE_QUOTE");
	if (type == TOKEN_DOUBLE_QUOTE)
		return ("DOUBLE_QUOTE");
	if (type == TOKEN_VARIABLE)
		return ("VARIABLE");
	if (type == TOKEN_PARAM)
		return ("PARAM");
	if (type == TOKEN_FILE_PATH)
		return ("FILE_PATH");
	if (type == TOKEN_NUMBER)
		return ("NUMBER");
	if (type == TOKEN_BACKSLASH)
		return ("BACKSLASH");
	if (type == TOKEN_AND)
		return ("AND");
	if (type == TOKEN_OR)
		return ("OR");
	return ("UNKNOWN");
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "ls", 2) == 0)
		return (1);
	else if (ft_strncmp(str, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(str, "echo", 4) == 0)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "export", 5) == 0)
		return (1);
	else if (ft_strncmp(str, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0)
		return (1);
	return (0);
}

t_token	*tokenize(char *input, int *token_count)
{
	t_token			*output;
	int				i;
	int				t_amount;
	char			*token;
	int				id;

	i = 0;
	t_amount = count_tokens(input);
	// CHANGED: I added +50 to catch-fit operators that got split into multiple tokens
	// or any difference in count and tokenizing -- avoid overflow
	// feel free to bring it back if the count is matching
	output = malloc(sizeof(t_token) * (t_amount + 51));
	if (!output)
		return (NULL);
	id = 0;
	// while (t_amount--) //COMMENTED OUT --> not using token count now
	// CHANGED to just keep scan string until end
	while(input[i])
	{
		skip_spaces(&i, input);
		if (!input[i]) // ADDED ESCAPE: NOTHING ELSE TO READ -> break
			break;
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
