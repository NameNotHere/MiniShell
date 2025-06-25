/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 12:58:55 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
	- NOT validate things that are supposed to fail in execve (ex: invalid path)
	- return clean AST node
	TODO: TOKEN # MUST COINCIDE WITH ARGV #! (so inside cmd, space or quote
	separated words MUST be equal to token number OR there must be a way to
	convert tokens to unify the separated ones (so no loss of information about
	space separation is allowed, or argv is not reacreatable.))
	TODO: REMOVE PRINTF DEBUGS
	// TODO:
	// 1. validade syntax,
	// 2. validate built in options (check: do we still run other commands?)
	// 4. remove quotes if needed & expand vars,
	// 5. cleanup
*/
void	parse_cmd(t_ast_node *ast, t_token *tokens, int start, int end)
{
	printf("cmd node->ADD\n" \
		"	start cmd tk: %d, end cmd tk: %d\n",
		start,
		end);
	ast->nty = NODE_CMD;
	parse_redir(ast, tokens, &start, &end);
	if (end > start)
		printf("		$ cmd is:%s, ends with %s\n",
			tokens[start].word, tokens[end - 1].word);
	else
		printf("		$ cmd is:%s, ends with (none)\n",
			tokens[start].word);
	ast->cmd.built_in = false;
	ast->cmd.argv = token_words_to_argv(tokens, start, end);
	return ;
}

char	**token_words_to_argv(t_token *tokens, int start, int end)
{
	int		i;
	char	**argv;

	argv = ft_calloc(end - start + 2, sizeof(char *));
	i = -1;
	while (++i + start < end)
		argv[i] = ft_strdup(tokens[i + start].word);
	i = -1;
	printf(" :: argv -> ");
	while (argv[++i] != NULL)
		printf("|%s", argv[i]);
	printf("|\n");
	return (argv);
}
