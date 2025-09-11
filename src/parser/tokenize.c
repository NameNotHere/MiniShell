/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/11 18:12:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_file_path(char *str, int *y)
{
	int	i;

	i = *y;
	if (str[0] == '.' || str[0] == '/' || str[0] == '~')
		return (1);
	else
	{
		while (str[i] && str[i] != ' ' && str[i] != '\n')
		{
			if (str[i++] == '.')
			{
				*y = i;
				return (1);
			}
		}
	}
	return (0);
}

int	search_for_singlequote(char *str)
{
	char	*s;

	s = str;
	s++;
	while (*s)
	{
		if (*s == '\'')
		{
			if (is_closed(s, 1, '\'') == 0)
				return (1);
		}
		s++;
	}
	return (0);
}

// "zz'zz" is not properly tokenisisng
void	tokenise_quotes(char *str, t_token *output)
{
	if (ft_strncmp(str, "\"", 1) == 0)
	{
		output->ty = TOKEN_SINGLE_QUOTE;
		if (search_for_singlequote(str) == 1)
		{
			if (search_for_singlequote(str) == 0)
				output->ty = UNCLOSED_DOUBLE_QUOTE;
			else
				output->ty = TOKEN_DOUBLE_QUOTE;
		}
	}
}

void	tokenise_redirs(char *str, t_token *output)
{
	if (ft_strncmp(str, "<<", 2) == 0)
		output->ty = TOKEN_HEREDOC;
	else if (ft_strncmp(str, ">>", 2) == 0)
		output->ty = TOKEN_APPEND;
	else if (ft_strncmp(str, "<", 1) == 0)
		output->ty = TOKEN_INPUT;
	else if (ft_strncmp(str, ">", 1) == 0)
		output->ty = TOKEN_OUTPUT;
	else if (str[0] == '|')
		output->ty = TOKEN_PIPE;
}

t_token	ft_token(char *str)
{
	t_token	output;
	int		i;

	i = 0;
	output.word = str;
	output.ty = TOKEN_WORD;
	if (ft_strncmp(str, "\'", 1) == 0)
		output.ty = TOKEN_SINGLE_QUOTE;
	else if (str[0] == '-')
		output.ty = TOKEN_DASH_PARAM;
	else if (str[0] == '$')
		output.ty = TOKEN_VARIABLE;
	else if (ft_strncmp(str, "&&", 2) == 0)
		output.ty = TOKEN_AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		output.ty = TOKEN_OR;
	else if (str[0] == '=')
		output.ty = TOKEN_EQUAL;
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
	if (type == TOKEN_EQUAL)
		return ("EQUAL");
	if (type == UNCLOSED_SINGLE_QUOTE)
		return ("UNCLOSED_SINGLE_QUOTE");
	if (type == UNCLOSED_DOUBLE_QUOTE)
		return ("UNCLOSED_DOUBLE_QUOTE");
	return ("UNKNOWN");
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "cd", 2) == 0)
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

/*
TODO: delete this comment
Added error code for caller to receive.
call with &err on an int err variable.
renamed output for res (short for result for norminette lines)
*/
t_token	*tokenize(char *input, int *token_count, int *err)
{
	t_token			*res;
	int				i;
	char			*token;
	int				id;

	i = 0;
	id = 0;
	*err = callo_x((void **)&res, sizeof(t_token), (count_tokens(input) + 2));
	if (*err)
		return (NULL);
	while (input[i])
	{
		skip_spaces(&i, input);
		if (!input[i])
			break ;
		token = make_word(input, &i, err);
		if (token)
			res[id++] = ft_token(token);
		else
			return (free(res), NULL);
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
