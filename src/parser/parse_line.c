/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 00:56:43 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(t_msh *sh, t_ast *ast, char *string)
{
	int			token_count;
	t_token		*tokens;
	int			lookup_result;

	tokens = tokenize(string, &token_count, &sh->err);
	if (!tokens)
	{
		msg_err("tokenizer failed");
		sh->exit_code = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	sh->tokens = tokens;
	build_ast(sh, ast, tokens);
	free_tokens(&sh->tokens, token_count);
	lookup_result = lookup_all_cmd_fullpaths(sh, sh->ast);
	if (lookup_result != EXIT_SUCCESS)
		sh->exit_code = lookup_result;
	return (EXIT_SUCCESS);
}
