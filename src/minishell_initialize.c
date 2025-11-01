/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:12:45 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/01 02:48:23 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	initialize_environment(t_msh *sh, char **envp)
{
	if (envp[0] == NULL && initialize_null_env(sh) != EXIT_SUCCESS)
		return (sh->exit_code);
	else if (envp[0])
		sh->envp = copy_string_array(envp);
	if (!sh->envp)
		return (ret_exit_perr(sh, ERRNO_CODE, E_INIT_ENV_MSG));
	if (update_path_dirs(&sh->path_dirs, sh->envp) != EXIT_SUCCESS)
		return (ret_exit_perr(sh, ERRNO_CODE, E_INIT_ENV_MSG));
	update_shell_level_var(sh);
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

static int	initialize_run_command(t_msh *sh, char **argv)
{
	int	pipefd[2];

	sh->is_interact = false;
	if (pipe(pipefd) < 0)
		return (ret_exit_perr(sh, ERRNO_CODE, "pipe"));
	if (write(pipefd[1], argv[2], ft_strlen(argv[2])) < 0)
		return (ret_exit_perr(sh, ERRNO_CODE, "write"));
	close(pipefd[1]);
	sh->script_fd = pipefd[0];
	return (sh->exit_code);
}

int	initialize_minishell(t_msh *sh, int argc, char **argv, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	if (initialize_environment(sh, envp) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (argc > 1 && !ft_strncmp(argv[1], "-c", 3))
	{
		if (argc < 3)
			return (ret_exit_msg(sh, 127, E_OPTION_C_ARGUMENT));
		return (initialize_run_command(sh, argv));
	}
	else if (argc > 1)
		return (initialize_run_script(sh, argv));
	sh->is_interact = isatty(STDIN_FILENO);
	if (sh->is_interact)
		write(STDOUT_FILENO, BRACKET_PASTE_CODE, ft_strlen(BRACKET_PASTE_CODE));
	sh->script_fd = -1;
	return (sh->exit_code);
}
