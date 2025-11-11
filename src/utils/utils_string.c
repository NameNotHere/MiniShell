/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:42:11 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 16:29:51 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
