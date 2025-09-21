/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:33:25 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/21 02:18:33 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"
#include <stdbool.h>

/*
	g_sig, t_sa and t_handler:
		- explained in minishell_signal header
*/

volatile sig_atomic_t	g_sig = 0;

// /**
//  * @brief Simple signal handler that just stores the signal number
//  * @param sig The signal number that was received (SIGINT, SIGQUIT, etc.)
//  *
//  * This handler "ignores" signals by:
//  * 1. Preventing the default action (like terminating the program)
//  * 2. Storing which signal occurred in g_sig for later processing
//  * 3. Allowing the program to continue running normally
//  */
// static void	sig_ignore(int sig)
// {
// 	g_sig = sig;
// }

// /**
//  * @brief Restores default signal handling behavior
//  * @return true if successful, false on error
//  *
//  * Used in child processes (like when executing external commands) to:
//  * 1. Reset g_sig to 0 (clear any previous signals)
//  * 2. Set SIGINT, SIGQUIT, SIGTERM back to their default behaviors
//  * 3. This means Ctrl+C will terminate the child process normally
//  * 4. The child won't inherit the parent's custom signal handlers
//  *
//  * Example: When you run "sleep 10" and press Ctrl+C, this ensures
//  * the sleep command gets terminated instead of the shell handling it.
//  */
// bool	default_signal_handler(void)
// {
// 	t_sa	sa;

// 	g_sig = 0;
// 	return (init_sig(&sa, 0, SIG_DFL));
// }

// /**
//  * @brief Sets up signal handling to gracefully ignore signals
//  * @return true if successful, false on error
//  *
//  * Used when the shell wants to catch signals but not terminate:
//  * 1. Resets g_sig to 0 (clear any previous signals)
//  * 2. Installs sig_ignore as the handler for SIGINT, SIGQUIT, SIGTERM
//  * 3. When signals occur, they're caught and stored in g_sig
//  * 4. The main program can check g_sig and decide what to do
//  *
//  * Example: During command execution, this prevents Ctrl+C from
//  * killing the shell itself, allowing proper cleanup.
//  */
// bool	ignore_signal_handler(void)
// {
// 	t_sa	sa;

// 	g_sig = 0;
// 	return (init_sig(&sa, 0, sig_ignore));
// }

// /**
//  * @brief Sets up signal handling with special SIGPIPE handling
//  * @return true if successful, false on error
//  *
//  * Used in pipeline operations where SIGPIPE needs special treatment:
//  * 1. Sets up ignore handling for SIGINT, SIGQUIT, SIGTERM (like above)
//  * 2. ADDITIONALLY sets up ignore handling specifically for SIGPIPE
//  * 3. SIGPIPE occurs when writing to a broken pipe (reader closed)
//  * 4. Without this, writing to a closed pipe would terminate the shell
//  *
//  * Example: In "ls | head -1", if head exits early, ls might get SIGPIPE
//  * when trying to write more output. This prevents ls (and the shell) from
//  * crashing.
//  */
// bool	ignore_sigpipe_handler(void)
// {
// 	t_sa	sa;

// 	g_sig = 0;
// 	return (init_sig(&sa, 0, sig_ignore)
// 		&& sigaction(SIGPIPE, &sa, NULL) != -1);
// }

// /**
//  * @brief Core function that installs a signal handler for multiple signals
//  * @param sa Pointer to sigaction structure to configure
//  * @param flags Signal behavior flags (usually 0 for basic handling)
//  * @param handler Function to call when signals are received
//  * @return true if all signal installations succeeded, false otherwise
//  *
//  * This function sets up signal handling for the "big three" signals:
//  * 1. SIGINT (Ctrl+C) - Interrupt signal
//  * 2. SIGQUIT (Ctrl+\) - Quit signal
//  * 3. SIGTERM - Termination signal (from kill command)
//  *
//  * Steps performed:
//  * 1. Configure the sigaction structure with flags and handler function
//  * 2. Clear the signal mask (don't block any signals during handler execution)
//  * 3. Install the same handler for all three signals
//  * 4. Return true only if ALL installations succeeded
//  */
// bool	init_sig(t_sigaction *sa, int flags, t_handler handler)
// {
// 	sa->sa_flags = flags;
// 	sa->sa_handler = handler;
// 	return (sigemptyset(&(sa->sa_mask)) != -1
// 		&& sigaction(SIGINT, sa, NULL) != -1
// 		&& sigaction(SIGQUIT, sa, NULL) != -1
// 		&& sigaction(SIGTERM, sa, NULL) != -1);
// }