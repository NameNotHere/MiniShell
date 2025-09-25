/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 17:50:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 21:32:04 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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