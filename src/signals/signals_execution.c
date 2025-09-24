/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 05:02:31 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 05:18:48 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"

/*
	signal handler to ignore signals

	signals will not have their usual effects
	(program keeps running as if no signal was received)
	signal is however stored for possible use later
 */
static void	handler_sig_ignore(int sig)
{
	g_sig = sig;
}

/*
	sets: ignore signal handling (SIGINT, SIGQUIT, SIGTERM)

	resets stored signal to 0
	uses install_sig_handler to install ignore signal handlers

	Returns:
	- true if install succeeded
	- false if install failed
 */
bool	set_ignore_sig(void)
{
	t_sa	sa;

	g_sig = 0;
	return (install_sig_handler(&sa, 0, handler_sig_ignore));
}

/*
	sets: restore default signal handling

	resets stored signal to 0
	uses install_sig_handler to install default handlers

	Returns:
	- true if install succeeded
	- false if install failed
 */
bool	set_restore_dfl_sig(void)
{
	t_sa	sa;

	g_sig = 0;
	return (install_sig_handler(&sa, 0, SIG_DFL));
}

/*
	sets: ignore signal handling plus SIGPIPE handling

	same as "set_ignore_sig", but ALSO sets SIGPIPE handling

	resets stored signal to 0
	uses install_sig_handler to install:
		- ignore signal handlers (SIGINT, SIGQUIT, SIGTERM)
		- ignore SIGPIPE (avoid crashing on pipe write errors)

	Returns:
	- true if install succeeded
	- false if install failed
 */
bool	set_ignore_sigpipe(void)
{
	t_sa	sa;

	g_sig = 0;
	return (install_sig_handler(&sa, 0, handler_sig_ignore)
		&& sigaction(SIGPIPE, &sa, NULL) != -1);
}
