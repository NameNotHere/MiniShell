/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/16 01:54:14 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO: make sure tokenizer error will produce exit code on error*/
int	parse_line_to_ast(t_msh *sh, t_ast *ast, char *string)
{
	int			token_count;
	t_token		*tokens;

	tokens = tokenize(string, &token_count, &sh->err);
	if (!tokens)
	{
		put_stderr("tokenizer failed");
		if (sh->exit_code == EXIT_SUCCESS)
			sh->exit_code = EXIT_FAILURE;
		return (sh->exit_code);
	}
	sh->tokens = tokens;
	build_ast(sh, ast, tokens);
	free_tokens(&sh->tokens, token_count);
	if (sh->exit_code == EXIT_SUCCESS)
		sh->exit_code = lookup_all_cmd_fullpaths(sh, sh->ast);
	return (sh->exit_code);
}
