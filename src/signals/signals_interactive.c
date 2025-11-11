/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 17:15:30 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 14:33:32 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <readline/readline.h>
#include <unistd.h>
#include <termios.h>

/*
	**helper for handler_interactive_sig, on int sig (control-c)
 	makes readline to stop input and show new prompt

	note: setting rl_done = 1 does that with the help of mainloop
	catching the signal also and looping a continue.
 */
static void	interactive_sig_int(void)
{
	rl_done = 1;
}

/*
	helper for handler_interactive_sig, on quit sig (control-\)
 	makes readline to redisplay previous input but on a new line

	write call uses ANSI code to clear current line of the terminal output and
	to move the cursor to the beginning of that line.
 */
static void	interactive_sig_quit(void)
{
	(void)write(STDOUT_FILENO, CLEAR_LINE_ANSI_CODE, sizeof(CLEAR_LINE_ANSI_CODE));
	rl_on_new_line();
	rl_redisplay();
}

/*
	signal handler for interactive shell

	stores signal and calls handler helper on two cases:
		signal is SIGINT (control-c)
		signal is SIGQUIT (control-\)
 */
static void	handler_interactive_sig(int sig)
{
	g_sig = sig;
	if (sig == SIGINT)
		interactive_sig_int();
	else if (sig == SIGQUIT)
		interactive_sig_quit();
}

/*
	sets: interactive signal handling

	uses install_sig_handler to install interactive signal handler

	Returns:
	- true if install succeeded.
	- false if install failed.
 */
bool	set_interactive_sig(void)
{
	t_sa			sa;

	return (install_sig_handler(&sa, 0, handler_interactive_sig));
}
