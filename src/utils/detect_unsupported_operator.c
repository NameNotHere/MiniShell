/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_unsupported_operator.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 13:05:01 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 12:49:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns the token type if an unsupported operator token exists:
	- TOKEN_AND, TOKEN_OR, TOKEN_AMPERSAND (logical operators)
	- TOKEN_SEMICOLON (command separator)
	- TOKEN_LPAREN, TOKEN_RPAREN (subshells/grouping)
	Otherwise return 0.

	- This is used by the parser to surface syntax errors for operators
	which are not supported by this minishell.
 */
int	detect_unsupported_operator(t_token *tokens)
{
	int	i;

	if (!tokens)
		return (EXIT_SUCCESS);
	i = 0;
	while (tokens[i].word)
	{
		if (tokens[i].ty == TOKEN_AND
			|| tokens[i].ty == TOKEN_OR
			|| tokens[i].ty == TOKEN_AMPERSAND
			|| tokens[i].ty == TOKEN_SEMICOLON
			|| tokens[i].ty == TOKEN_LPAREN
			|| tokens[i].ty == TOKEN_RPAREN)
			return (tokens[i].ty);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	process_unsupported_operator_error(t_msh *sh)
{
	int	token_type;

	token_type = detect_unsupported_operator(sh->tokens);
	if (token_type == TOKEN_OR)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SYNTAX_OR));
	if (token_type == TOKEN_AND)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SYNTAX_AND));
	if (token_type == TOKEN_AMPERSAND)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SYNTAX_AMPERSAND));
	if (token_type == TOKEN_SEMICOLON)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SEMICOLON));
	if (token_type == TOKEN_LPAREN)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SYNTAX_LPAREN));
	if (token_type == TOKEN_RPAREN)
		return (r_set_exit_msg(sh, EXIT_SYNTAX, E_SYNTAX_RPAREN));
	return (EXIT_SYNTAX);
}
