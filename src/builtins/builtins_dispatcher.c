/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatcher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:20:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	int	ret;

	ret = EXIT_CMD_NOT_FOUND;
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ret = x_pwd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ret = x_cd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ret = x_echo(cmd->argv, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ret = x_env(sh, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ret = x_export(sh, *cmd);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ret = x_unset(sh, *cmd);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ret = x_exit(sh, *cmd);
	return (ret);
}
