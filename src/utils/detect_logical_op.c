/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_logical_op.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 13:05:01 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 14:47:06 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns TOKEN_AND if a TOKEN_AND token exists
	Returns TOKEN_OR if a TOKEN_OR token exists
	Otherwise return 0.

	- This is intentionally simple and used by the parser to surface a syntax
	error for logical operators which are not supported by this minishell.
 */
int	detect_logical_op_token(t_token *tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i].word)
	{
		if (tokens[i].ty == TOKEN_AND
			|| tokens[i].ty == TOKEN_OR
			|| tokens[i].ty == TOKEN_AMPERSAND)
			return (tokens[i].ty);
		i++;
	}
	return (0);
}

int	process_logical_op_syntax_error(t_msh *sh)
{
	int	logop;

	logop = detect_logical_op_token(sh->tokens);
	if (logop == TOKEN_OR)
		return (r_set_exit_msg(sh, 2, E_SYNTAX_ERROR_OR));
	if (logop == TOKEN_AND)
		return (r_set_exit_msg(sh, 2, E_SYNTAX_ERROR_AND));
	if (logop == TOKEN_AMPERSAND)
		return (r_set_exit_msg(sh, 2, E_SYNTAX_ERROR_AMPERSAND));
	return (2);
}
