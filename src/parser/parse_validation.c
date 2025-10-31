/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 14:47:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/30 11:03:20 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_has_cmd(int *i, bool *has_cmd, t_token *tokens)
{
	if (is_valid_cmd_token(tokens[*i].ty))
		*has_cmd = true;
	(*i)++;
}

/*
	validate_pipe_syntax: checks if pipe has valid commands on both sides
	Returns:
		- 0 if syntax is valid
		- 2 if syntax error
*/
int	validate_pipe_syntax(t_token *tokens, int start, int end)
{
	int		i;
	bool	found_pipe;
	bool	has_left_cmd;
	bool	has_right_cmd;

	i = start;
	found_pipe = false;
	has_left_cmd = false;
	has_right_cmd = false;
	while (tokens[i].word && i <= end && tokens[i].ty != TOKEN_PIPE)
		set_has_cmd(&i, &has_left_cmd, tokens);
	if (tokens[i].ty == TOKEN_PIPE)
	{
		found_pipe = true;
		i++;
		while (tokens[i].word && i <= end)
			set_has_cmd(&i, &has_right_cmd, tokens);
	}
	if (found_pipe && (!has_left_cmd || !has_right_cmd))
		return (2);
	return (0);
}

/*
	validate_semicolon_syntax: checks if any semicolon tokens are present
	Returns:
		- 0 if no semicolons found
		- 2 if semicolon found (syntax error for minishell)
*/
int	validate_semicolon_syntax(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].word)
	{
		if (ft_strncmp(tokens[i].word, ";", 1) == 0)
			return (2);
		i++;
	}
	return (0);
}
