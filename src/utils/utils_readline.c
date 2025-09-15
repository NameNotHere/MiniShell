/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_readline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:07:24 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 14:03:10 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
TODO: remove this if UNUSED (likely since we never needed to come back to this)
This function is used to read a line from the terminal and not interfering
with the standard input/output pipes.
	opens /dev/tty for reading and writing
	replaces stdin and stdout with /dev/tty
	uses the readline library to read a line from the terminal
		passes a string or NULL to the line pointer;
	restores stdin and stdout to their original values (protects pipes)
	returns 0 on success
┌──────────────┐
│ readline_on_ │
│     _tty()   │
└────┬──┬──────┘
     │  │
     │  └── STDOUT_FILENO → /dev/tty (the terminal)
     └───── STDIN_FILENO  → /dev/tty (the keyboard)

TODO: maybe check if we can just use the regular plain readline without
issues with interference with pipes and heredocs.
TODO: check if we can use this wrapper (just) for heredocs (if it is advantage)
TODO: check if the shell of minishell is supposed to receive information from
	STDIN (not tty) anyway (then we CANNOT wrap/protect it).
*/
bool	readline_on_tty(const char *prompt, char **line)
{
	int		fd_tty_in;
	int		fd_tty_out;
	int		duped_stdin;
	int		duped_stdout;

	fd_tty_in = open("/dev/tty", O_RDONLY);
	fd_tty_out = open("/dev/tty", O_WRONLY);
	if (fd_tty_in == -1 || fd_tty_out == -1)
		return (false);
	duped_stdin = dup(STDIN_FILENO);
	duped_stdout = dup(STDOUT_FILENO);
	dup2(fd_tty_in, STDIN_FILENO);
	dup2(fd_tty_out, STDOUT_FILENO);
	*line = readline(prompt);
	dup2(duped_stdin, STDIN_FILENO);
	dup2(duped_stdout, STDOUT_FILENO);
	close(fd_tty_in);
	close(fd_tty_out);
	close(duped_stdin);
	close(duped_stdout);
	return (true);
}
