/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 17:23:15 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 16:54:44 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_everything(t_msh *sh)
{
	free_ast(&sh->ast);
	safe_free_string(&sh->line);
	safe_free_2d_string(&sh->envp);
	safe_free_2d_string(&sh->path_dirs);
	if (sh->script_fd >= 0)
		close(sh->script_fd);
	rl_clear_history();
}

/*
	Shell line cleanup
	Free/Reset AST and line.
*/
void	shell_line_cleanup(t_msh *sh)
{
	free_ast(&sh->ast);
	sh->ast = make_ast_node(NODE_UNKNOWN);
	safe_free_string(&sh->line);
	sh->saved_exit_code = sh->exit_code;
	sh->exit_code = EXIT_SUCCESS;
}
