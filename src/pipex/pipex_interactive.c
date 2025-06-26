/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_interactive.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 17:58:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 12:15:02 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "pipex.h"
/*
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
     │  └── stdout → /dev/tty (the terminal)
     └───── stdin  → /dev/tty (the keyboard)
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

/*
TODO: signal handling
*/
int	run_pipex_interactive(const char *arg_0, char **envp)
{
	char	*line;
	int		argc;
	char	**argv;
	bool	run_it;

	while (1)
	{
		if (!readline_on_tty("pipex_i> ", &line))
			return (put_stderr("terminal (tty) not available\n"), 1);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			safe_free_string(&line);
			break ;
		}
		run_it = true;
		if (!ft_strlen(line) || !get_args_from_line(line, arg_0, &argc, &argv))
			run_it = false;
		if (run_it)
			run_pipex_once(argc, argv, envp);
		safe_free_2d_string(&argv);
		safe_free_string(&line);
	}
	safe_free_string(&line);
	// free(line = readline(""));
	rl_clear_history();
	return (0);
}

bool	get_args_allocation_error(char *reason, char ***line_argv)
{
	put_stderr("memory allocation failed for ");
	put_stderr(reason);
	safe_free_2d_string(line_argv);
	return (false);
}

bool	get_args_from_line(char *line, const char *arg_0, \
			int *argc, char ***argv)
{
	char	**line_argv;
	int		line_argc;
	int		i;

	line_argv = split_charset_using_quote(line, " ");
	if (!line_argv)
		return (get_args_allocation_error("split\n", NULL));
	line_argc = 0;
	while (line_argv[line_argc] != NULL)
		line_argc++;
	*argv = malloc(sizeof(char *) * (line_argc + 2));
	if (!*argv)
		return (get_args_allocation_error("argv\n", &line_argv));
	(*argv)[0] = ft_strdup(arg_0);
	i = -1;
	while (++i < line_argc)
		(*argv)[i + 1] = ft_strdup(line_argv[i]);
	(*argv)[line_argc + 1] = NULL;
	*argc = line_argc + 1;
	safe_free_2d_string(&line_argv);
	return (true);
}
