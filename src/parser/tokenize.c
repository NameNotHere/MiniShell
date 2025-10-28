/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/10/28 14:05:10 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

void	ft_second_token(char *str, t_token *output)
{
	if (ft_strncmp(str, "\'", 1) == 0)
		output->ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-')
		output->ty = TOKEN_DASH_PARAM;
	else if (str[0] == '$')
		output->ty = TOKEN_VARIABLE;
	else if (ft_strncmp(str, "&&", 2) == 0)
		output->ty = TOKEN_AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		output->ty = TOKEN_OR;
	else if (ft_strncmp(str, "&", 1) == 0)
		output->ty = TOKEN_AMPERSAND;
}

t_token	ft_token(char *str)
{
	t_token	output;
	int		i;

	i = 0;
	output.word = str;
	output.ty = TOKEN_WORD;
	ft_second_token(str, &output);
	if (output.ty != TOKEN_WORD)
		return (output);
	else if (is_file_path(str, &i))
		output.ty = TOKEN_FILE_PATH;
	else if (is_builtin(str) == 1)
		output.ty = TOKEN_INBUILT;
	else if (ft_isdigit(str[0]))
	{
		while (str[i])
		{
			if (ft_isdigit(str[i]))
				output.ty = TOKEN_NUMBER;
			i++;
		}
	}
	tokenise_redirs(str, &output);
	tokenise_quotes(str, &output);
	return (output);
}

const char	*get_token_name(t_token_ty type)
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
		return ("TOKEN_DOUBLE_QUOTE");
	return (get_token_name_continued(type));
}

const char	*get_token_name_continued(t_token_ty type)
{
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
	if (type == UNCLOSED_SINGLE_QUOTE)
		return ("UNCLOSED_SINGLE_QUOTE");
	if (type == UNCLOSED_DOUBLE_QUOTE)
		return ("UNCLOSED_DOUBLE_QUOTE");
	return ("UNKNOWN");
}

static void	strip_exp_marks(char *str)
{
	int	i;
	int	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != EXP_MARK)
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

t_token	*tokenize(char *input, int *token_count, int *err)
{
	t_token			*res;
	int				i;
	char			*token;
	int				id;

	i = 0;
	id = 0;
	*err = x_calloc((void **)&res, sizeof(t_token), (count_tokens(input, 0, 0) + 2));
	if (*err)
		return (NULL);
	while (input[i])
	{
		skip_spaces(&i, input);
		if (!input[i])
			break ;
		token = make_word(input, &i, err);
		if (token)
		{
			res[id] = ft_token(token);
			strip_exp_marks(res[id].word);
			id++;
		}
		else
			return (free(res), NULL);
	}
	*token_count = id;
	return (res);
}
