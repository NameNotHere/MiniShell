/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 13:48:31 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line(t_msh *sh, t_ast *ast, char *string)
{
	int			token_count;
	t_token		*tokens;
	int			ret;

	tokens = tokenize(string, &token_count, &sh->exit_code);
	if (!tokens)
		return (r_set_exit_msg(sh, EXIT_FAILURE, E_TOKENIZE_FAILED));
	sh->tokens = tokens;
	ret = build_ast(sh, ast, tokens);
	if (ret != EXIT_SUCCESS)
	{
		free_tokens(&sh->tokens, token_count);
		if (ret == EXIT_SYNTAX)
		{
			sh->exit_code = EXIT_SYNTAX;
			return (EXIT_SYNTAX);
		}
		return (r_set_exit_msg(sh, EXIT_FAILURE, E_AST_BUILD_FAILED));
	}
	free_tokens(&sh->tokens, token_count);
	ret = lookup_all_cmd_fullpaths(sh, sh->ast);
	if (ret != EXIT_SUCCESS)
	{
		sh->exit_code = ret;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
