/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 01:47:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_flow	cycle_loop(t_msh *sh)
{
	sh->line = get_shell_line(sh, MSH_PROMPT);
	if (sh->line == NULL && sh->is_interact)
		write(1, "exit\n", 5);
	if (sh->line == NULL)
		return (BREAK_FLOW);
	if (g_sig == SIGINT)
		return (set_exit_code(sh, 130), CONTINUE_FLOW);
	if (!*sh->line)
		return (CONTINUE_FLOW);
	if (unclosed_quotes(sh->line))
		return (set_exit_msg(sh, 2, E_UNCLOSED_QUOTES_MSG), CONTINUE_FLOW);
	if (has_complex_heredoc_delimiter(sh->line))
		return (set_exit_msg(sh, 2, E_HDOC_DELIM_MSG), BREAK_FLOW);
	if (sh->is_interact)
		add_history(sh->line);
	if (!expand_string_variables(sh, &sh->line))
		return (set_exit_code(sh, EXIT_FAILURE), CONTINUE_FLOW);
	if (parse_line(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		return (msg_err("parse line failed\n"), CONTINUE_FLOW);
	return (EXEC_FLOW);
}

static int	minishell_mainloop(t_msh *sh)
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

int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;

	if (envp[0] == NULL)
	{
		msg_err("error: empty environment variables\n");
		return (EXIT_FAILURE);
	}
	if (initialize_minishell(&sh, argc, argv, envp) != EXIT_SUCCESS)
		return (sh.exit_code);
	if (minishell_mainloop(&sh) != EXIT_SUCCESS)
		return (sh.exit_code);
	return (sh.exit_code);
}
