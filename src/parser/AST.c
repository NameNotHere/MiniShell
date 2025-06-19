/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/19 10:05:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell_parser.h"

/*
AST DATASTRUCTURE ADDED TO minishell.h
*/


// TODO: ADD A FUNCTION TO PARSE THE WHOLE THING THEN for each command:
// find start/end token, then parse the command there (func below)

/*
CMD only tokens sent here (knowing start and end of cmd tokens):
	- 1st - will extract command name (with or without path)
	- process arguments (expand variables, remove quotes)
	- detect redirections (extract all redir tokens from CMD parsing and use
	them to add redir_in_node or redir_out_node)
	- validate most things (syntax errors, invalid built in params)
	- NOT validate things that are supposed to fail in execve (ex: invalid path)
	- return clean AST node
*/
t_ast_node	*parse_command_tokens(t_token *tokens, int start, int end)
{
	(void)tokens;
	printf("parsing command tokens now\nstart token: %d, end token: %d\n",
		start,
		end);
	return (NULL);
}
