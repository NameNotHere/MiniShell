/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_shell_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:17:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Handles continuation input for unclosed quotes.
	Returns updated full string with new line appended, or NULL on error.
*/

static char	*handle_unclosed_quotes(char **line, char *full)
{
	char	*tmp;

	safe_free_str(line);
	*line = readline("unclosed quotes> ");
	if (!*line)
		return (NULL);
	tmp = ft_strjoin3(full, "\n", *line);
	safe_free_str(&full);
	return (tmp);
}

/*
	Gets a line in interactive mode with readline.
	Handles unclosed quotes by prompting for continuation.

	Returns a line string typed in interactive mode.
	If failed, returns NULL.
*/

static char	*get_interactive_line(t_msh *sh, char *prompt)
{
	char	*line;
	char	*full;

	g_sig = 0;
	if (!set_interactive_sig())
	{
		msg_err(E_SIGNAL_INTERACTIVE);
		sh->exit_code = EXIT_FAILURE;
		return (NULL);
	}
	rl_event_hook = event_hook_sigint_return;
	line = readline(prompt);
	if (!line)
		return (NULL);
	if (!PRO || !unclosed_quotes(line))
		return (line);
	full = ft_strdup(line);
	while (unclosed_quotes(full))
	{
		full = handle_unclosed_quotes(&line, full);
		if (!full)
			break ;
	}
	safe_free_str(&line);
	return (full);
}

/*
	Gets a line in non-interactive mode.
	Reads from script_fd or stdin.

	Returns a line string from file/pipe (stripped of newline char).
	If failed, returns NULL.
*/

static char	*get_noninteractive_line(t_msh *sh)
{
	char	*line;
	int		input_fd;

	line = NULL;
	if (sh->script_fd >= 0)
		input_fd = sh->script_fd;
	else
		input_fd = STDIN_FILENO;
	if (g_sig != SIGINT && g_sig != SIGQUIT
		&& set_ignore_sig()
		&& readline_noninteract(input_fd, &sh->readbuf, &line) == false)
		safe_free_str(&line);
	return (line);
}

/*
	Gets a line in two different possible modes depending on is_interactive flag
	- interactive (with readline)
	- non-interactive (with readline_noninteractive)

	Returns a line string typed in interactive mode, or string passed through
	pipe (stripped of newline char).
	If failed, returns NULL.
*/

char	*get_shell_line(t_msh *sh, char *prompt)
{
	if (sh->is_interact)
		return (get_interactive_line(sh, prompt));
	else
		return (get_noninteractive_line(sh));
}
