/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/04 17:33:40 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_token	ft_token(char *str)
{
	t_token	output;

	output.word = str;
	output.ty = TOKEN_WORD;
	if (ft_strncmp(str, "&&", 2) == 0)
		output.ty = TOKEN_AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		output.ty = TOKEN_OR;
	else if (ft_strncmp(str, "&", 1) == 0)
		output.ty = TOKEN_AMPERSAND;
	else if (str[0] == '-')
		output.ty = TOKEN_DASH_PARAM;
	else if (is_builtin(str) == 1)
		output.ty = TOKEN_INBUILT;
	else
		tokenise_redirs(str, &output);
	return (output);
}

const char	*get_token_name(t_token_ty type)
{
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
	if (type == TOKEN_DASH_PARAM)
		return ("DASH_PARAM");
	if (type == TOKEN_AND)
		return ("AND");
	if (type == TOKEN_OR)
		return ("OR");
	if (type == TOKEN_AMPERSAND)
		return ("AMPERSAND");
	return ("UNKNOWN");
}

const char	*get_token_name_continued(t_token_ty type)
{
	return (get_token_name(type));
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
	x_calloc((void **)&res, err, sizeof(t_token), (count_tokens(input, 0, 0) + 2));
	if (*err)
		return (NULL);
	while (input[i])
	{
		skip_spaces(&i, input);
		if (!input[i])
			break ;
		token = make_word(input, &i, err);
		if (!token)
			return (free(res), NULL);
		res[id] = ft_token(token);
		strip_exp_marks(res[id].word);
		id++;
	}
	*token_count = id;
	return (res);
}
