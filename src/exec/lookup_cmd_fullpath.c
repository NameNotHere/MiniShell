/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookup_cmd_fullpath.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 06:11:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/28 15:12:30 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: remove comment below
// NEW: now NULL argv[0] is valid (redirection only), so we just return.
void	lookup_cmd_fullpath(t_msh *sh, t_cmd *cmd)
{
	if (cmd->built_in)
		return ;
	if (cmd->argv[0] == NULL || !ft_strlen(cmd->argv[0]))
		return ;
	cmd->full_cmd = get_valid_cmd_full_path(sh->path_dirs, cmd->argv[0]);
	if (cmd->full_cmd == NULL)
	{
		cmd->full_cmd = ft_strdup(cmd->argv[0]);
		if (cmd->full_cmd == NULL)
			cmd->full_cmd = get_empty_string();
		if (ft_strchr(cmd->argv[0], '/') && !access(cmd->argv[0], F_OK))
			cmd->permission_denied = true;
		else
			cmd->not_found = true;
	}
}

int	lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node)
{
	if (!node)
	{
		put_stderr("error: on lookup cmd paths, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		lookup_cmd_fullpath(sh, &node->cmd);
	else if (node->nty == NODE_PIPE)
	{
		lookup_all_cmd_fullpaths(sh, node->pipe.left);
		lookup_all_cmd_fullpaths(sh, node->pipe.right);
	}
	return (EXIT_SUCCESS);
}
