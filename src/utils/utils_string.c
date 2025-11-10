/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:42:11 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 13:48:31 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	*line;
	char	*full;
	char	*tmp;
	int		input_fd;

	line = NULL;
	full = NULL;

	if (sh->is_interact)
	{
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
		full = ft_strdup(line);
		while (PRO && unclosed_quotes(full))
		{
			safe_free_str(&line);
			line = readline("unclosed quotes> ");
			if (!line)
				break ;
			tmp = ft_strjoin3(full, "\n", line);
			safe_free_str(&full);
			full = tmp;
		}
		safe_free_str(&line);
		return (full);
	}
	else
	{
		if (sh->script_fd >= 0)
			input_fd = sh->script_fd;
		else
			input_fd = STDIN_FILENO;
		if (g_sig != SIGINT && g_sig != SIGQUIT
			&& set_ignore_sig()
			&& readline_noninteract(input_fd, &sh->readbuf, &line) == false)
			safe_free_str(&line);
	}
	return (line);
}

/*
	Adds a line to a string, after newline char.
	If string is NULL, string is copy of the line.
*/
int	add_line_to_string(char **string, char **line)
{
	int		ret;
	char	*updated_string;

	ret = EXIT_SUCCESS;
	if (!(*line))
	{
		msg_err(E_ADD_LINE_INVALID);
		return (EXIT_FAILURE);
	}
	if (*string)
		updated_string = ft_strjoin3(*string, "\n", *line);
	else
		updated_string = ft_strdup(*line);
	if (updated_string == NULL)
	{
		msg_perr(E_ADD_LINE_STRING);
		ret = EXIT_FAILURE;
	}
	safe_free_str(string);
	safe_free_str(line);
	*string = updated_string;
	updated_string = NULL;
	return (ret);
}

/*
	Returns an empty string
	TODO: check if we need to catch error here or on the caller.
	Probably the latter
*/
char	*get_empty_string(void)
{
	return (ft_calloc(1, sizeof(char)));
}

bool	set_empty_string(char **to_empty)
{
	safe_free_str(to_empty);
	*to_empty = get_empty_string();
	if (*to_empty == NULL)
		return (false);
	return (true);
}
