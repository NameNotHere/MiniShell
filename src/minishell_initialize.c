/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:12:45 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 01:17:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	initialize_environment(t_msh *sh, char **envp)
{
	sh->envp = copy_string_array(envp);
	if (!sh->envp)
		return (ret_exit_perr(sh, ERRNO_CODE, "initialize_environment"));
	sh->path_dirs = ft_split(get_path_from_env(sh->envp), ':');
	if (!sh->path_dirs)
		return (ret_exit_perr(sh, ERRNO_CODE, "initialize_environment"));
	return (sh->exit_code);
}

static int	initialize_run_script(t_msh *sh, char **argv)
{
	sh->is_interact = false;
	sh->script_fd = open(argv[1], O_RDONLY);
	if (sh->script_fd < 0)
	{
		msg_err_2("minishell: ", argv[1]);
		return (ret_exit_perr(sh, ERRNO_CODE, ""));
	}
	return (sh->exit_code);
}

int	initialize_minishell(t_msh *sh, int argc, char **argv, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	if (initialize_environment(sh, envp) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (argc > 1)
		return (initialize_run_script(sh, argv));
	sh->is_interact = isatty(STDIN_FILENO);
	sh->script_fd = -1;
	return (sh->exit_code);
}
