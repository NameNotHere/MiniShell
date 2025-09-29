/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/29 04:32:28 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;

	if (envp[0] == NULL)
	{
		put_stderr("error: empty environment variables\n");
		return (EXIT_FAILURE);
	}
	if (initialize_minishell(&sh, argc, argv, envp) != EXIT_SUCCESS)
		return (sh.exit_code);
	if (minishell_mainloop(&sh) != EXIT_SUCCESS)
		return (sh.exit_code);
	return (sh.exit_code);
}

/*
TODO: handle ENOMEM (Out of memory) error for path dirs
*/
int	initialize_minishell(t_msh *sh, int argc, char **argv, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	sh->envp = copy_string_array(envp);
	sh->path_dirs = ft_split(get_path_from_env(sh->envp), ':');
	if (argc > 1)
	{
		sh->is_interact = false;
		sh->script_fd = open(argv[1], O_RDONLY);
		if (sh->script_fd < 0)
		{
			put_stderr_2("minishell: ", argv[1]);
			perror("");
			sh->exit_code = EXIT_FAILURE;
			return (sh->exit_code);
		}
	}
	else
	{
		sh->is_interact = isatty(STDIN_FILENO);
		sh->script_fd = -1;
	}
	if (!sh->path_dirs || !sh->envp)
	{
		put_stderr("initialize_minishell: out of memory error");
		sh->exit_code = ENOMEM;
	}
	return (sh->exit_code);
}

t_flow	cycle_loop(t_msh *sh)
{
	sh->line = get_shell_line(sh, MSH_PROMPT);
	if (sh->line == NULL && sh->is_interact)
		write(1, "exit\n", 5);
	if (sh->line == NULL)
		return (BREAK_FLOW);
	if (g_sig == SIGINT)
	{
		sh->exit_code = 130;
		return (CONTINUE_FLOW);
	}
	if (!*sh->line)
		return (CONTINUE_FLOW);
	if (unclosed_quotes(sh->line))
		return (put_stderr_code(sh, "error: unclosed quotes\n", 2),
			CONTINUE_FLOW);
	if (has_complex_heredoc_delimiter(sh->line))
	{
		put_stderr_code(sh, "syntax error: complex quoted heredoc "
			"delimiters not supported\n", 2);
		return (BREAK_FLOW);
	}
	if (sh->is_interact)
		add_history(sh->line);
	if (!expand_string_variables(sh, &sh->line))
	{
		sh->exit_code = EXIT_FAILURE;
		return (CONTINUE_FLOW);
	}
	if (parse_line(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		return (put_stderr("parse line failed\n"), CONTINUE_FLOW);
	return (EXEC_FLOW);
}

int	minishell_mainloop(t_msh *sh)
{
	t_flow	flow;

	while (true)
	{
		shell_line_cleanup(sh);
		flow = cycle_loop(sh);
		if (flow == BREAK_FLOW)
			break ;
		else if (flow == CONTINUE_FLOW)
			continue ;
		sh->exit_code = exec_ast(sh, sh->ast, STDIN_FILENO, STDOUT_FILENO);
		sh->saved_exit_code = sh->exit_code;
	}
	free_everything(sh);
	return (sh->exit_code);
}
