/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/20 20:56:47 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// volatile sig_atomic_t	g_sig = 0;

int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;
	t_sa	sa;

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
	return (sh.exit_code);
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
	sh->is_interact = isatty(STDIN_FILENO);
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
	g_sig = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

int	minishell_mainloop(t_msh *sh)
{
	while (true)
	{
		sh->line = get_shell_line(sh, MSH_PROMPT);
		if (sh->line == NULL && sh->is_interact)
			write(1, "exit\n", 5);
		if (sh->line == NULL)
			break ;
		if (!*sh->line && make_string_free(&sh->line))
			continue ;
		if (sh->is_interact)
			add_history(sh->line);
		if (expand_line(sh) != EXIT_SUCCESS && make_string_free(&sh->line))
			continue ;
		if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		{
			put_stderr("parse line failed\n");
			safe_free_string(&sh->line);
			continue ;
		}
		if (exec_ast(sh, sh->ast, STDIN_FILENO, STDOUT_FILENO) != EXIT_SUCCESS)
			put_stderr("exec ast root failed\n");
		shell_line_cleanup(sh);
	}
	free_everything(sh);
	return (sh->exit_code);
}
