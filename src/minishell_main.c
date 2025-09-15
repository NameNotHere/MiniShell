/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 17:46:56 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_mini_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_msh				sh;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	sa.sa_handler = ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	if (envp[0] == NULL)
	{
		put_stderr("error: empty environment variables\n");
		sh.exit_code = EXIT_FAILURE;
		return (sh.exit_code);
	}
	if (initialize_minishell(&sh, envp) != EXIT_SUCCESS)
		return (sh.exit_code);
	if (minishell_mainloop(&sh) != EXIT_SUCCESS)
		return (sh.exit_code);
	return (EXIT_SUCCESS);
}

/*
TODO: handle ENOMEM (Out of memory) error for path dirs
*/
int	initialize_minishell(t_msh *sh, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	sh->envp = copy_string_array(envp);
	sh->ast = make_ast_node(NODE_UNKNOWN);
	sh->path_dirs = ft_split(get_path_from_env(sh->envp), ':');
	sh->is_interactive = isatty(STDIN_FILENO);
	if (!sh->path_dirs || !sh->envp)
	{
		put_stderr("initialize_minishell: out of memory error");
		sh->exit_code = ENOMEM;
	}
	return (sh->exit_code);
}

void	ctrl_c(int sig)
{
	(void)sig;
	g_mini_signal = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/*
	TODO: Remove the exit handling from mainloop, it needs to run as a builtin.
*/
int	minishell_mainloop(t_msh *sh)
{
	while (true)
	{
		sh->line = get_shell_line(sh->is_interactive, MINISHELL_PROMPT);
		if ((!sh->line || !*sh->line) && make_string_free(&sh->line))
			continue ;
		if (sh->is_interactive)
			add_history(sh->line);
		if (expand_line(sh) != EXIT_SUCCESS && make_string_free(&sh->line))
			continue ;
		if (ft_strncmp(sh->line, "exit", 4) == 0 && !piped_line(sh->line))
		{
			write(1, "exit\n", 5);
			free_everything(sh);
			sh->exit_code = EXIT_SUCCESS;
			break ;
		}
		if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		{
			perror("parse line");
			safe_free_string(&sh->line);
			continue ;
		}
		if (lookup_all_cmd_fullpaths(sh, sh->ast) != EXIT_SUCCESS)
			perror("lookup_cmds");
		if (exec_ast(sh, sh->ast,
				STDIN_FILENO, STDOUT_FILENO) != EXIT_SUCCESS)
			perror("exec ast root");
		shell_line_cleanup(sh);
	}
	free_everything(sh);
	return (sh->exit_code);
}
