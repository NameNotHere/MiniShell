/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_types.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/04 14:13:23 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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
