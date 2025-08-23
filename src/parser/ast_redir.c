/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:13:32 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 22:11:34 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
TODO: remove printfs, add error handling
TODO: check when empty command is valid, if always (redir only is valid in bash)s
*/

int invalid_redir(t_msh *sh, int i)
{
	// you need to find the bounds max
	if (i <= 0 || !sh->tokens[i + 1].word)
		return (1);
	if (!sh->tokens[i - 1].word) // need to make sure its a command or arg
		return (1);
	i++;
	if (!sh->tokens[i].word)
		return (1);
	if (get_redir_type(sh->tokens[i].ty) != REDIR_UNKNOWN)
		return (1);
	if (isminioperator(sh->tokens[i].word, 0))
		return (1);
	return (0);
}

void	parse_redir(t_msh *sh,  t_ast *ast, int *start, int *end)
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
		if (sh->tokens[i].ty == TOKEN_INPUT || \
			sh->tokens[i].ty == TOKEN_HEREDOC || \
			sh->tokens[i].ty == TOKEN_APPEND || \
			sh->tokens[i].ty == TOKEN_OUTPUT)
		{
			if (invalid_redir(sh, i) == 1)
				a_print(" ***ERROR*** " \
					"invalid redirection");
			if (!before_cmd)
				*end = i;
			before_cmd = true;
			add_redir(ast, sh->tokens[i].ty, sh->tokens[i + 1].word);
			i++;
		}
		else
		{
			if (before_cmd)
			{
				cmd_count++;
				before_cmd = false;
				*start = i;
				if (cmd_count > 1)
					a_print(" *** ERROR *** "\
					"invalid syntax, multiple commands!\n");
				else
					*start = i;
			}
		}
	}
}

// TODO: HANDLE ALLOC ERRORS for ft_calloc & ft_strdup
// TODO: maybe: check valid ast and word
void	add_redir(t_ast *ast, t_token_ty token_type, char *word)
{
	t_redir	*current_redir;
	t_redir	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->string = ft_strdup(word);
	if (!new_redir->string)
	{
		free(new_redir);
		return ;
	}
	new_redir->ty = get_redir_type(token_type);
	a_print("__redir: ty %d : %s\n", new_redir->ty, word);
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

t_redir_ty	get_redir_type(t_token_ty ty)
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