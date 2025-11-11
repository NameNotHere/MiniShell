/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:35:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_single_var(t_msh *sh, char *name)
{
	int	i;
	int	env_len;

	if (name[0] == '-')
	{
		msg_err_3(E_UNSET_START, name, E_UNSET_END);
		return (EXIT_SYNTAX);
	}
	i = search_name(name, sh->envp);
	if (i == -1)
		return (EXIT_SUCCESS);
	free(sh->envp[i]);
	env_len = envp_len(sh->envp);
	while (i < env_len - 1)
	{
		sh->envp[i] = sh->envp[i + 1];
		i++;
	}
	sh->envp[env_len - 1] = NULL;
	if (ft_strcmp(name, "PATH") == 0)
		update_path_dirs(&sh->path_dirs, sh->envp);
	return (EXIT_SUCCESS);
}

int	x_unset(t_msh *sh, t_cmd cmd)
{
	int	arg_idx;
	int	exit_code;
	int	ret;

	if (!cmd.argv[1])
		return (EXIT_SUCCESS);
	arg_idx = 1;
	exit_code = EXIT_SUCCESS;
	while (arg_idx < cmd.argc)
	{
		ret = unset_single_var(sh, cmd.argv[arg_idx]);
		if (ret != EXIT_SUCCESS)
			exit_code = ret;
		arg_idx++;
	}
	return (exit_code);
}
