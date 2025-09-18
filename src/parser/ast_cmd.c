/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/18 18:19:39 by tda-roch         ###   ########.fr       */
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

bool	init_remove_quotes(char *str, char **result, int len)
{
	*result = NULL;
	if (!str)
		return (false);
	if (len == 0)
	{
		*result = get_empty_string();
		return (false);
	}
	*result = ft_calloc(len + 1, sizeof(char));
	if (!*result)
	{
		perror("init remove quotes");
		return (false);
	}
	return (true);
}

char	*remove_quotes(char *str, int len)
{
	char	*result;
	int		str_i;
	int		res_i;
	bool	in_single_quote;
	bool	in_double_quote;

	if (init_remove_quotes(str, &result, len) == false)
		return (result);
	str_i = 0;
	res_i = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (str[str_i])
	{
		if (str[str_i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[str_i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			result[res_i++] = str[str_i];
		str_i++;
	}
	return (result);
}

char	**token_words_to_argv(t_token *tokens, int start, int end)
{
	int		i;
	char	**argv;
	char	*word;

	argv = ft_calloc(end - start + 2, sizeof(char *));
	i = -1;
	while (++i + start < end)
	{
		word = tokens[i + start].word;
		argv[i] = remove_quotes(word, ft_strlen(word));
	}

	i = -1;
	a_print(" :: argv -> ");
	while (argv[++i] != NULL)
		a_print("|%s", argv[i]);
	a_print("|\n");
	return (argv);
}
