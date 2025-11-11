/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_token_ty	get_token_type(char *str)
{
	if (VALIDATE && ft_strncmp(str, "&&", 2) == 0)
		return (TOKEN_AND);
	else if (VALIDATE && ft_strncmp(str, "||", 2) == 0)
		return (TOKEN_OR);
	else if (VALIDATE && str[0] == '&')
		return (TOKEN_AMPERSAND);
	else if (VALIDATE && str[0] == ';')
		return (TOKEN_SEMICOLON);
	else if (VALIDATE && str[0] == '(')
		return (TOKEN_LPAREN);
	else if (VALIDATE && str[0] == ')')
		return (TOKEN_RPAREN);
	else if (str[0] == '|')
		return (TOKEN_PIPE);
	else if (ft_strncmp(str, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (TOKEN_APPEND);
	else if (str[0] == '<')
		return (TOKEN_INPUT);
	else if (str[0] == '>')
		return (TOKEN_OUTPUT);
	else if (str[0] == '-')
		return (TOKEN_DASH_PARAM);
	return (TOKEN_WORD);
}

t_token	make_token(char *str)
{
	t_token	output;

	output.word = str;
	output.ty = get_token_type(str);
	return (output);
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
	char			*token_word;
	int				id;

	i = 0;
	id = 0;
	if (xe_calloc_token(&res, err, count_tokens(input, 0, 0) + 2))
		return (NULL);
	while (input[i])
	{
		skip_spaces(&i, input);
		if (!input[i])
			break ;
		token_word = make_token_word(input, &i, err);
		if (!token_word)
			return (r_free_null((void **)&res));
		res[id] = make_token(token_word);
		strip_exp_marks(res[id].word);
		id++;
	}
	*token_count = id;
	return (res);
}

void	free_tokens(t_token **tokens, int amount)
{
	int	i;

	if (!tokens || !*tokens)
		return ;
	i = 0;
	while (amount--)
	{
		free((*tokens)[i].word);
		i++;
	}
	free(*tokens);
	*tokens = NULL;
}
