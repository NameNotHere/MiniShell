/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:29:17 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 05:18:31 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <readline/readline.h>

/*
	signal handler for heredocs
	heredoc will only handle sigint (or "control-c")
	if control-c/sigint: aborts heredoc, returns to shell
 */
static void	handler_heredoc_sig(int sig)
{
	g_sig = sig;
	rl_done = 1;
}

/*
	sets signal handler for heredocs

	similar to the generic "install_sig_handler", but takes no parameters
	since this function just serves to set/install handler_heredoc_sig

	return:
		true if successful install
		false if failed install
 */
bool	set_heredoc_sig(void)
{
	t_sa	sa;

	sa.sa_flags = 0;
	sa.sa_handler = handler_heredoc_sig;
	return (
		sigemptyset(&(sa.sa_mask)) != -1
		&& sigaction(SIGINT, &sa, NULL) != -1);
}
