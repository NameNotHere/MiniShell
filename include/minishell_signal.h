/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:55:54 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/07 11:10:17 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H

# include <signal.h>
# include <stddef.h>
# include <stdbool.h>

/*
	this escape code printed on a terminal is used to tell terminal to clear
	current line and return cursor to the start of the line
*/
# define CLEAR_LINE_ANSI_CODE "\r\033[2K"

// Signal integer value received by minishell.
//
// Only global variable in this project.
// extern + volatile + sig_atomic are needed for different reasons:
// extern:
// 	prevents each file that includes this header to create a new variable
// volatile:
//	prevents compiler optimizations to assume the variable is constant and
// 	prevents using cached values, etc.
// sig_atomic:
// makes sure changes in the integer value is done atomically, so on read
// or write the value will be correct.
extern volatile sig_atomic_t	g_sig;

// signal action struct
typedef struct sigaction		t_sa;

// function pointer for signal handlers
typedef void					(*t_handler)(int);

// signals/signals.c
bool	install_sig_handler(t_sa *sa, int flags, t_handler handler);
int		event_hook_sigint_return(void);

// signals/signals_execution.c
bool	set_ignore_sig(void);
bool	set_restore_dfl_sig(void);
bool	set_ignore_sigpipe(void);

// signals/signals_heredoc.c
bool	set_heredoc_sig(void);

// signals/signals_interactive.c
bool	set_interactive_sig(void);
#endif