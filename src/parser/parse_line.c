/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 14:47:06 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(t_msh *sh, t_ast *ast, char *string)
{
	int			token_count;
	t_token		*tokens;
	int			lookup_result;

	tokens = tokenize(string, &token_count, &sh->exit_code);
	if (!tokens)
		return (r_set_exit_msg(sh, EXIT_FAILURE, E_TOKENIZE_FAILED));
	sh->tokens = tokens;
	if (build_ast(sh, ast, tokens) != EXIT_SUCCESS)
	{
		free_tokens(&sh->tokens, token_count);
		if (sh->exit_code != 2)
			return (r_set_exit_msg(sh, EXIT_FAILURE, E_AST_BUILD_FAILED));
		else
			return (EXIT_FAILURE);
	}
	free_tokens(&sh->tokens, token_count);
	lookup_result = lookup_all_cmd_fullpaths(sh, sh->ast);
	if (lookup_result != EXIT_SUCCESS)
	{
		sh->exit_code = lookup_result;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
