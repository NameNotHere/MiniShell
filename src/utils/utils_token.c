/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:50:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/26 13:39:34 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
	Returns true if token is a redirect token
*/
bool	is_redir_token(t_token_ty token_type)
{
	if (token_type == TOKEN_INPUT
		|| token_type == TOKEN_OUTPUT
		|| token_type == TOKEN_APPEND
		|| token_type == TOKEN_HEREDOC)
		return (true);
	return (false);
}

/*
	used to check in a list of command tokens, if a token is
	within redirections tokens (so either is a redir token,
	or is the next token right after)
*/
bool	is_within_redir_tokens(t_token *tokens, int i)
{
	if (is_redir_token(tokens[i].ty))
		return (true);
	if (i > 0 && is_redir_token(tokens[i - 1].ty))
		return (true);
	return (false);
}

/*
	Returns true if the token type represents a valid command argument
	(but not a pipe token itself).
	NOTES:
	Valid command argument types:
	- TOKEN_WORD: regular words/strings
	- TOKEN_INBUILT: builtin commands
	- TOKEN_NUMBER: numeric arguments
	- TOKEN_FILE_PATH: file paths
	- TOKEN_DASH_PARAM: parameters starting with dash (like -n)
	Returns false for pipe tokens and other special tokens.
*/
bool	is_valid_cmd_token(t_token_ty token_type)
{
	return (token_type == TOKEN_WORD
		|| token_type == TOKEN_INBUILT
		|| token_type == TOKEN_NUMBER
		|| token_type == TOKEN_FILE_PATH
		|| token_type == TOKEN_DASH_PARAM);
}
