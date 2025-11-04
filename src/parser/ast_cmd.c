/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 14:06:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include "minishell.h"

void	parse_cmd(t_msh *sh, t_ast *ast, int start, int end)
{
	ast->nty = NODE_CMD;
	parse_redir(sh, ast, &start, &end);
	if (sh->exit_code != EXIT_SUCCESS)
		return ;
	ast->cmd.argv = token_words_to_argv(sh->tokens, start, end, 0);
	ast->cmd.argc = 0;
	while (ast->cmd.argv && ast->cmd.argv[ast->cmd.argc])
		ast->cmd.argc++;
	ast->cmd.built_in = false;
	if (ast->cmd.argv && ast->cmd.argv[0] && is_builtin(ast->cmd.argv[0]))
		ast->cmd.built_in = true;
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
		ms_perror(E_INIT_REMOVE_QUOTES);
		return (false);
	}
	return (true);
}

char	*remove_quotes(char *str, int len)
{
	char	*result;
	int		str_i;
	int		res_i;
	bool	in_sgl_quote;
	bool	in_dbl_quote;

	if (init_remove_quotes(str, &result, len) == false)
		return (result);
	in_sgl_quote = false;
	in_dbl_quote = false;
	res_i = 0;
	str_i = -1;
	while (str[++str_i])
	{
		if (str[str_i] == '\'' && !in_dbl_quote && !is_escaped(str, str_i))
			in_sgl_quote = !in_sgl_quote;
		else if (str[str_i] == '"' && !in_sgl_quote && !is_escaped(str, str_i))
			in_dbl_quote = !in_dbl_quote;
		else if (str[str_i] == '\\' && !in_sgl_quote && str[str_i + 1]
			&& (str[str_i + 1] == '"' || str[str_i + 1] == '\''))
			result[res_i++] = str[++str_i];
		else
			result[res_i++] = str[str_i];
	}
	return (result);
}

char	**token_words_to_argv(t_token *tokens, int start, int end, int argc)
{
	int		i;
	int		token_i;
	char	**argv;
	char	*word;

	i = start;
	while (i < end)
		if (tokens[i].word && !is_within_redir_tokens(tokens, i++))
			argc++;
	argv = ft_calloc(argc + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	token_i = start;
	while (token_i < end && i < argc)
	{
		if (tokens[token_i].word && !is_within_redir_tokens(tokens, token_i))
		{
			word = tokens[token_i].word;
			argv[i++] = remove_quotes(word, ft_strlen(word));
		}
		token_i++;
	}
	return (argv);
}
