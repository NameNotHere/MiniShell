/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 04:13:32 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/26 13:33:33 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
TODO: remove printfs, add error handling
TODO: check when empty command is valid, if always (redir only is valid in bash)s
*/

int	invalid_redir(t_msh *sh, int i)
{
	if (i <= 0 || !sh->tokens[i + 1].word)
		return (1);
	if (!sh->tokens[i - 1].word || !sh->tokens[i + 1].word)
		return (1);
	i++;
	if (get_redir_type(sh->tokens[i].ty) != REDIR_UNKNOWN)
		return (1);
	if (ft_strchr("&;()*?#", (int)(sh->tokens[i].word[0])))
		return (1);
	return (0);
}

void	parse_redir(t_msh *sh,  t_ast *ast, int *start, int *end)
{
	int				i;
	bool			cmd_found;

	i = *start - 1;
	cmd_found = false;
	while (++i < *end)
	{
		if (is_redir_token(sh->tokens[i].ty))
		{
			if (invalid_redir(sh, i) == 1)
			{
				a_print(" ***ERROR*** \n\t\tinvalid redirection");
				break ;
			}
			add_redir(ast, sh->tokens[i].ty, sh->tokens[i + 1].word);
			i++;
		}
		else if (!cmd_found)
		{
			*start = i;
			cmd_found = true;
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
	new_redir->string = remove_quotes(word, ft_strlen(word));
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