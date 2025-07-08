/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 22:13:55 by tda-roch         ###   ########.fr       */
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
/*
// void	parse_cmd(t_msh *sh, t_ast *ast, int start, int end)
// {
// 	// a_print("cmd node->ADD\n" \
// 	// 	"	start cmd tk: %d, end cmd tk: %d\n",
// 	// 	start,
// 	// 	end);
// 	ast->nty = NODE_CMD;
// 	parse_redir(sh, ast, &start, &end);
// 	// if (end > start)
// 	// 	a_print("		$ cmd is:%s, ends with %s\n",
// 	// 		sh->tokens[start].word, sh->tokens[end - 1].word);
// 	// else
// 	// 	a_print("		$ cmd is:%s, ends with (none)\n",
// 	// 		sh->tokens[start].word);
// 	ast->cmd.built_in = false;
// 	ast->cmd.argv = token_words_to_argv(sh->tokens, start, end);
// 	return ;
// }
*/
void	parse_cmd(t_msh *sh, t_ast *ast, int start, int end)
{
	ast->nty = NODE_CMD;
	parse_redir(sh, ast, &start, &end);
	ast->cmd.built_in = false;
	ast->cmd.argv = token_words_to_argv(sh->tokens, start, end);
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
	a_print(" :: argv -> ");
	while (argv[++i] != NULL)
		a_print("|%s", argv[i]);
	a_print("|\n");
	return (argv);
}
