/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_signal.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 19:55:54 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/20 20:25:35 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_SIGNAL_H
# define MINISHELL_SIGNAL_H

# include <signal.h>
# include <stddef.h>

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

#endif