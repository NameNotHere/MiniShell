/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 14:47:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/28 15:08:31 by tda-roch         ###   ########.fr       */
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

/* Check if line contains complex heredoc delimiters

	TODO: replace strstr
	TODO: add the initial check for strstr here instead of in mainloop
*/
bool	has_complex_heredoc_delimiter(char *line)
{
	char	*heredoc_pos;
	char	*delimiter_start;
	int		i;
	int		quote_count;
	bool	has_variables;

	heredoc_pos = strstr(line, "<<");
	while (heredoc_pos)
	{
		delimiter_start = heredoc_pos + 2;
		while (*delimiter_start == ' ' || *delimiter_start == '\t')
			delimiter_start++;
		i = 0;
		quote_count = 0;
		has_variables = false;
		while (delimiter_start[i] && delimiter_start[i] != ' '
			&& delimiter_start[i] != '\t' && delimiter_start[i] != '\n')
		{
			if (delimiter_start[i] == '"' || delimiter_start[i] == '\'')
				quote_count++;
			if (delimiter_start[i] == '$')
				has_variables = true;
			i++;
		}
		if (quote_count > 2 && has_variables)
			return (true);
		heredoc_pos = strstr(heredoc_pos + 2, "<<");
	}
	return (false);
}
