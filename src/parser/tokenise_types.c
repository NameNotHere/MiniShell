/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/12 21:38:29 by tda-roch         ###   ########.fr       */
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