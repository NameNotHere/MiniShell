/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:42:11 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 05:45:32 by tda-roch         ###   ########.fr       */
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

	line = NULL;
	if (sh->is_interact)
	{
		g_sig = 0;
		if (!set_interactive_sig())
		{
			put_stderr("set interactive signal handling failed");
			sh->exit_code = EXIT_FAILURE;
			return (NULL);
		}
		line = readline(prompt);
	}
	else if (g_sig != SIGINT && g_sig != SIGQUIT
		&& set_ignore_sig()
		&& readline_noninteract(STDIN_FILENO, &sh->readbuf, &line) == false)
		safe_free_string(&line);
	return (line);
}

/*
	Adds a line to a string, after newline char.
	If string is NULL, string is copy of the line.
*/
int	add_line_to_string(char **string, char **line)
{
	int		result;
	char	*updated_string;

	result = EXIT_SUCCESS;
	if (!(*line) || !*(*line))
	{
		put_stderr("add line to string: invalid line");
		return (EXIT_FAILURE);
	}
	if (*string)
		updated_string = ft_strjoin3(*string, "\n", *line);
	else
		updated_string = ft_strdup(*line);
	if (updated_string == NULL)
	{
		perror("add line to string");
		result = EXIT_FAILURE;
	}
	safe_free_string(string);
	safe_free_string(line);
	*string = updated_string;
	updated_string = NULL;
	return (result);
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

/*
	Sets a string to an empty string.
	Returns true on success, false on failure.

*/
bool	set_empty_string(char **to_empty)
{
	safe_free_string(to_empty);
	*to_empty = get_empty_string();
	if (*to_empty == NULL)
		return (false);
	return (true);
}
