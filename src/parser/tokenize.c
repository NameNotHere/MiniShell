/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/23 12:54:47 by tda-roch         ###   ########.fr       */
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
		output.ty = TOKEN_INPUT;
	else if (ft_strncmp(str, ">", 1) == 0)
		output.ty = TOKEN_OUTPUT;
	else if (ft_strncmp(str, "\"", 1) == 0)
		output.ty = TOKEN_DOUBLE_QUOTE;
	else if (ft_strncmp(str, "\'", 1) == 0)
		output.ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-')
		output.ty = TOKEN_DASH_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else if (ft_strncmp(str, "&&", 2) == 0)
		output.ty = TOKEN_AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		output.ty = TOKEN_OR;
	else if (str[0] == '|')
		output.ty = TOKEN_PIPE;
	else if (str[0] == '=')
		output.ty = TOKEN_EQUAL;
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

const char	*get_token_name(t_token_type type)
{
	if (type < 0 || type > TOKEN_LAST)
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
	if (type == TOKEN_DASH_PARAM)
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
	if (type == TOKEN_EQUAL)
		return ("EQUAL");
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
	char			*token;
	int				id;

	i = 0;
	output = malloc(sizeof(t_token) * (count_tokens(input)));
	if (!output)
		return (NULL);
	id = 0;
	while(input[i])
	{
		skip_spaces(&i, input);
		if (!input[i])
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
	output[id] = (t_token){0};
	*token_count = id;
	return (output);
}

void	free_tokens(t_token *tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i].word != NULL)
	{
		free(tokens[i].word);
		i++;
	}
	free(tokens);
}