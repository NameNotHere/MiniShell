/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:13:32 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 13:48:31 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include "minishell.h"

int	invalid_redir(t_msh *sh, int i)
{
	if (!sh->tokens[i + 1].word)
		return (EXIT_FAILURE);
	if (i > 0 && !sh->tokens[i - 1].word)
		return (EXIT_FAILURE);
	i++;
	if (get_redir_type(sh->tokens[i].ty) != REDIR_UNKNOWN)
		return (EXIT_FAILURE);
	if (ft_strchr("&;()*?#", (int)(sh->tokens[i].word[0])))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	parse_redir(t_msh *sh,  t_ast *ast, int *start, int *end)
{
	int				i;
	int				ret;
	bool			cmd_found;

	i = *start - 1;
	cmd_found = false;
	while (++i < *end)
	{
		if (is_redir_token(sh->tokens[i].ty))
		{
			if (invalid_redir(sh, i) == 1)
				return (r_msg_err(E_REDIR_INVALID, EXIT_SYNTAX));
			ret = add_redir(ast, sh->tokens[i].ty, sh->tokens[i + 1].word);
			if (ret != EXIT_SUCCESS)
				return (ret);
			i++;
		}
		else if (!cmd_found)
		{
			*start = i;
			cmd_found = true;
		}
	}
	return (EXIT_SUCCESS);
}

int	add_redir(t_ast *ast, t_token_ty token_type, char *word)
{
	t_redir	*current_redir;
	t_redir	*new_redir;

	if (x_calloc_redir(&new_redir, 1) != EXIT_SUCCESS)
		return (r_msg_perr(E_ALLOC_REDIR, EXIT_FAILURE));
	new_redir->quoted = has_quotes(word);
	new_redir->string = remove_quotes(word, ft_strlen(word));
	if (!new_redir->string)
	{
		safe_free((void **)&new_redir);
		return (EXIT_FAILURE);
	}
	new_redir->ty = get_redir_type(token_type);
	if (!ast->cmd.redir)
		ast->cmd.redir = new_redir;
	else
	{
		current_redir = ast->cmd.redir;
		while (current_redir->next)
			current_redir = current_redir->next;
		current_redir->next = new_redir;
	}
	return (EXIT_SUCCESS);
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
