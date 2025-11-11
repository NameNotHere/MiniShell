/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 14:33:32 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_flow	cycle_loop(t_msh *sh)
{
	sh->line = get_shell_line(sh, MSH_PROMPT);
	if (sh->line == NULL && sh->is_interact)
		(void)write(1, "exit\n", 5);
	if (sh->line == NULL)
		return (BREAK_FLOW);
	if (g_sig == SIGINT)
		return (r_set_exit_ret(sh, 130, CONTINUE_FLOW));
	if (!*sh->line)
		return (CONTINUE_FLOW);
	if (error_unclosed_quotes(sh->line))
		return (r_set_exit_ret(sh, 2, CONTINUE_FLOW));
	if (sh->is_interact)
		add_history(sh->line);
	if (!expand_string_variables(sh, &sh->line, false))
		return (r_set_exit_ret(sh, EXIT_FAILURE, CONTINUE_FLOW));
	if (parse_line(sh, sh->ast, sh->line) != EXIT_SUCCESS)
	{
		if (!sh->is_interact && sh->exit_code == 2)
			return (BREAK_FLOW);
		return (CONTINUE_FLOW);
	}
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
	return (r_free_everything(sh, sh->exit_code));
}

int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;

	if (initialize_minishell(&sh, argc, argv, envp) != EXIT_SUCCESS)
		return (r_free_everything(&sh, sh.exit_code));
	return (minishell_mainloop(&sh));
}
