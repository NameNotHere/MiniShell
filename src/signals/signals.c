/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:33:25 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 13:03:22 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <stdlib.h>

/*
	g_sig is a global variable to store last signal received
	- explained in more detail in minishell_signal.h (header)
*/
volatile sig_atomic_t	g_sig = 0;

/*
	installs a signal handler for 3 signals:
		- SIGINT (control-c)
		- SIGQUIT (control-\)
		- SIGTERM (kill command)
	a) clears all signal handling
	b) sets flags and handler to sigaction
	c) handler function is called when signal received

	Returns:
	- true if install succeeded
	- false if install failed
*/
bool	install_sig_handler(t_sa *sa, int flags, t_handler handler)
{
	sa->sa_flags = flags;
	sa->sa_handler = handler;
	return (sigemptyset(&(sa->sa_mask)) != -1
		&& sigaction(SIGINT, sa, NULL) != -1
		&& sigaction(SIGQUIT, sa, NULL) != -1
		&& sigaction(SIGTERM, sa, NULL) != -1);
}
/*
	Event hook for rl_event_hook.
	This event hook makes readline return on ctrl-c,
		with newline after SIGINT
*/
int	event_hook_sigint_return(void)
{
	return (EXIT_SUCCESS);
}
