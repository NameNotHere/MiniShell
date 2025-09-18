/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/18 11:14:43 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include "minishell.h"

/*
	TODO: TOKEN # MUST COINCIDE WITH ARGV #! (so inside cmd, space or quote
	separated words MUST be equal to token number OR there must be a way to
	convert tokens to unify the separated ones (so no loss of information about
	space separation is allowed, or argv is not reacreatable.))
	TODO: REMOVE PRINTF DEBUGS
*/
void	parse_cmd(t_msh *sh, t_ast *ast, int start, int end)
{
	ast->nty = NODE_CMD;
	parse_redir(sh, ast, &start, &end);
	ast->cmd.built_in = false;
	if (sh->tokens[start].ty == TOKEN_INBUILT)
		ast->cmd.built_in = true;
	ast->cmd.argv = token_words_to_argv(sh->tokens, start, end);
	return ;
}

char	*remove_quotes(char *str)
{
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
		return (ft_strndup(str + 1, len - 2));
	return (ft_strdup(str));
}

char	**token_words_to_argv(t_token *tokens, int start, int end)
{
	int		i;
	char	**argv;

	argv = ft_calloc(end - start + 2, sizeof(char *));
	i = -1;
	while (++i + start < end)
		argv[i] = remove_quotes(tokens[i + start].word);
	i = -1;
	a_print(" :: argv -> ");
	while (argv[++i] != NULL)
		a_print("|%s", argv[i]);
	a_print("|\n");
	return (argv);
}
