/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_initialize.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:12:45 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 11:11:13 by tda-roch         ###   ########.fr       */
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
		return (r_set_exit_perr(sh, E_INIT_ENV));
	if (update_path_dirs(&sh->path_dirs, sh->envp) != EXIT_SUCCESS)
		return (r_set_exit_perr(sh, E_INIT_ENV));
	update_shell_level_var(sh);
	return (sh->exit_code);
}

static int	initialize_run_script(t_msh *sh, char **argv)
{
	sh->is_interact = false;
	sh->script_fd = open(argv[1], O_RDONLY);
	if (sh->script_fd < 0)
	{
		msg_err(argv[1]);
		return (r_set_exit_perr(sh, E_OPEN_RUN_SCRIPT));
	}
	return (sh->exit_code);
}

static int	initialize_run_command(t_msh *sh, char **argv)
{
	int	pipefd[2];

	sh->is_interact = false;
	if (pipe(pipefd) < 0)
		return (r_set_exit_perr(sh, E_PIPE));
	if (write(pipefd[1], argv[2], ft_strlen(argv[2])) < 0)
		return (r_set_exit_perr(sh, E_WRITE));
	close(pipefd[1]);
	sh->script_fd = pipefd[0];
	return (sh->exit_code);
}

/*
	Sets default echo to control chars as true, so control+c (SIGINT)
	by default prints ^C. Minishell is handling SIGQUIT (triggered by control+\)
	in a different way, to avoid printing anything (or doing anything): just
	prints nothing always.

	Control char echoes (like ^C for SIGINT) can be turned off with:
		stty -echoctl
	back on with:
		stty echoctl

	NOTE: there is an issue with bracketed paste on the first line
	without this function call:
	rl_variable_bind("enable-bracketed-paste", "off");
	minishell messes up prompt if you paste on first line.
	using the second line fixes it, or adding this function call.
	but that function is not in the allowed functions list.
*/
static void	initialize_run_interactive(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag |= ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
	rl_on_new_line();
	rl_redisplay();
}

int	initialize_minishell(t_msh *sh, int argc, char **argv, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	if (initialize_environment(sh, envp) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (argc > 1 && ft_strncmp(argv[1], "-c", 3) == 0)
	{
		if (argc < 3)
			return (r_set_exit_msg(sh, 127, E_OPTION_C_ARGUMENT));
		return (initialize_run_command(sh, argv));
	}
	else if (argc > 1)
		return (initialize_run_script(sh, argv));
	sh->is_interact = isatty(STDIN_FILENO);
	if (sh->is_interact)
		initialize_run_interactive();
	sh->script_fd = -1;
	return (sh->exit_code);
}
