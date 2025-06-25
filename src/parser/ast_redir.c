/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:13:32 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 05:10:02 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
TODO: remove printfs, add error handling
TODO: check when empty command is valid, if always (redir only is valid in bash)s
*/
void	parse_redir(t_ast_node *ast, t_token *tokens, int *start, int *end)
{
	int				i;
	int				last_node_token;
	int				cmd_count;
	bool			before_cmd;

	cmd_count = 0;
	i = *start - 1;
	last_node_token = *end;
	before_cmd = true;
	while (++i < last_node_token)
	{
		if (tokens[i].ty == TOKEN_INPUT || tokens[i].ty == TOKEN_HEREDOC \
			|| tokens[i].ty == TOKEN_APPEND || tokens[i].ty == TOKEN_OUTPUT)
		{
			if (i == *end)
				printf(" ***ERROR*** " \
					"invalid redirection, needs a file or delimiter\n");
			if (!before_cmd)
				*end = i;
			before_cmd = true;
			add_redir(ast, tokens[i].ty, tokens[i + 1].word);
			i++;
		}
		else
		{
			if (before_cmd)
			{
				cmd_count++;
				before_cmd = false;
				if (cmd_count > 1)
					printf(" *** ERROR *** "\
					"invalid syntax, multiple commands!\n");
				else
					*start = i;
			}
		}
	}
}

// TODO: HANDLE ALLOC ERRORS for ft_calloc & ft_strdup
// TODO: maybe: check valid ast and word
void	add_redir(t_ast_node *ast, t_token_type token_type, char *word)
{
	t_redir_node	*current_redir;
	t_redir_node	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir_node));
	if (!new_redir)
		return ;
	new_redir->string = ft_strdup(word);
	if (!new_redir->string)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = get_redir_type(token_type);
	printf("__redir: ty %d : %s\n", new_redir->type, word);
	if (!ast->cmd.redir)
		ast->cmd.redir = new_redir;
	else
	{
		current_redir = ast->cmd.redir;
		while (current_redir->next)
			current_redir = current_redir->next;
		current_redir->next = new_redir;
	}
}

t_redir_ty	get_redir_type(t_token_type ty)
{
	if (ty == TOKEN_INPUT)
		return (REDIR_INPUT);
	if (ty == TOKEN_OUTPUT)
		return (REDIR_OUTPUT);
	if (ty == TOKEN_APPEND)
		return (REDIR_APPEND);
	if (ty == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_UNKNOWN);
}