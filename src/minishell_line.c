/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 11:17:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/30 15:44:28 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_line_and_execute_ast(t_msh *sh)
{
	if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		return (sh->err);
	printf("\n***checking command paths***\n");
	lookup_all_cmd_fullpaths(sh, sh->ast);
	execute_ast_node(sh, sh->ast, false);
	return (EXIT_SUCCESS);
}

/*
TODO: substitute "print_build_ast" with "build_ast" before EVALUATION
*/
int	parse_line_to_ast(t_msh *sh, t_ast *ast, char *string)
{
	int			i;
	int			token_count;
	t_token		*tokens;

	printf("Input string: %s\n", string);
	printf("Expected token count: %d\n", count_tokens(string));
	tokens = tokenize(string, &token_count, &sh->err);
	if (!tokens)
	{
		printf("tokenizer failed with error #%d\n", sh->err);
		return (sh->err);
	}
	printf("Actual token count: %d\n", token_count);
	i = -1;
	sh->tokens = tokens;
	while (token_count > ++i)
		printf("%2d %12s  %s \n",
			tokens[i].ty,
			get_token_name(tokens[i].ty),
			tokens[i].word);
	print_build_ast(sh, ast, tokens);
	free_tokens(tokens, token_count);
	return (EXIT_SUCCESS);
}
