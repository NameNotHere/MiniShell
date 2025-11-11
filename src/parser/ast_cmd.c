/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:49:37 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:57:27 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_cmd(t_msh *sh, t_ast *ast, int start, int end)
{
	int	ret;

	ast->nty = NODE_CMD;
	ret = parse_redir(sh, ast, &start, &end);
	if (ret != EXIT_SUCCESS)
		return (ret);
	ast->cmd.argv = token_words_to_argv(sh->tokens, start, end, 0);
	if (!ast->cmd.argv)
		return (EXIT_FAILURE);
	ast->cmd.argc = 0;
	while (ast->cmd.argv && ast->cmd.argv[ast->cmd.argc])
		ast->cmd.argc++;
	ast->cmd.built_in = false;
	if (ast->cmd.argv && ast->cmd.argv[0] && is_builtin(ast->cmd.argv[0]))
		ast->cmd.built_in = true;
	return (EXIT_SUCCESS);
}

bool	init_remove_quotes(char *str, char **result, t_remove_quotes *rq, int len)
{
	*result = NULL;
	if (!str)
		return (false);
	if (len == 0)
	{
		*result = get_empty_string();
		return (false);
	}
	if (x_calloc_char(result, len + 1) != EXIT_SUCCESS)
	{
		msg_perr(E_INIT_REMOVE_QUOTES);
		return (false);
	}
	ft_bzero(rq, sizeof(t_remove_quotes));
	rq->str_i = -1;
	return (true);
}

/*
	Advanced parsing (applies if PRO)
	On each str char, if no normal quotes were detected:
	- replaces ESCAPED_SGL_QUOTE
	or
	- skips escape char and adds sglquote or dblquote to result
	or (falback)
	- passes char from str to result
*/
void	remove_quotes_pro(char *str, char *result, t_remove_quotes *q)
{
	if (str[q->str_i] == ESCAPED_SGL_QUOTE)
		result[q->res_i++] = '\'';
	else if (str[q->str_i] == '\\' && !q->in_sgl_quote && str[q->str_i + 1]
		&& (str[q->str_i + 1] == '"' || str[q->str_i + 1] == '\''))
		result[q->res_i++] = str[++q->str_i];
	else
		result[q->res_i++] = str[q->str_i];
}

/*
	remove quotes from command argvs
	since quotes were preserved on expansion and tokenizing,
	they need to be stripped before making the argvs

	simple quote removal for the non-PRO case
	on PRO, also handles cases for preserving some escaped
*/
char	*remove_quotes(char *str, int len)
{
	char			*result;
	t_remove_quotes	q;

	if (init_remove_quotes(str, &result, &q, len) == false)
		return (result);
	while (str[++q.str_i])
	{
		if (str[q.str_i] == '\'' && !q.in_dbl_quote
			&& !is_escaped(str, q.str_i))
			q.in_sgl_quote = !q.in_sgl_quote;
		else if (str[q.str_i] == '"' && !q.in_sgl_quote
			&& !is_escaped(str, q.str_i))
			q.in_dbl_quote = !q.in_dbl_quote;
		else if (!PRO)
			result[q.res_i++] = str[q.str_i];
		else if (PRO)
			remove_quotes_pro(str, result, &q);

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
	if (x_calloc_charptr(&argv, argc + 1) != EXIT_SUCCESS)
		return (msg_perr_null("token words to argv: allocation"));
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
