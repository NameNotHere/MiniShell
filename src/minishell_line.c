/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 19:11:46 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*
TODO: we are returning either sh->err or sh-exit_code.
need to decide what (if what) something exits.
*/
int	parse_line_and_execute_ast(t_msh *sh)
{
	if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		return (sh->exit_code);
	d_print("\n***checking command paths***\n");
	lookup_all_cmd_fullpaths(sh, sh->ast);
	sh->exit_code = execute_ast_node(sh, sh->ast, STDIN_FILENO, STDOUT_FILENO);
	return (sh->exit_code);
}

/*
TODO: substitute "print_build_ast" with "build_ast" before EVALUATION
*/
int	parse_line_to_ast(t_msh *sh, t_ast *ast, char *string)
{
	int			i;
	int			token_count;
	t_token		*tokens;

	d_print("Input string: %s\n", string);
	d_print("Expected token count: %d\n", count_tokens(string));
	tokens = tokenize(string, &token_count, &sh->err);
	if (!tokens)
	{
		d_print("tokenizer failed with error #%d\n", sh->exit_code);
		return (sh->exit_code);
	}
	d_print("Actual token count: %d\n", token_count);
	i = -1;
	sh->tokens = tokens;
	while (token_count > ++i)
		d_print("%2d %12s  %s \n",
			tokens[i].ty,
			get_token_name(tokens[i].ty),
			tokens[i].word);
	print_build_ast(sh, ast, tokens);
	free_tokens(&sh->tokens, token_count);
	return (EXIT_SUCCESS);
}
