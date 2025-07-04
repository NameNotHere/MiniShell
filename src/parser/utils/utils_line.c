/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:49:57 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 14:17:37 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

/*
helper function for minishell_mainloop, to check if commands are piped or not.
	reads line string, looks for pipe char (except if in quotes).
	Returns:
		true if piped
		false if not
*/
bool	piped_line(char *line)
{
	int		i;
	char	quote;

	quote = 0;
	i = 0;
	while (line[i])
	{
		if (quote && quote == line[i])
			quote = 0;
		else if (quote)
			;
		else if (ft_strchr("\'\"", line[i]))
			quote = line[i];
		else if ('|' == line[i])
			return (true);
		i++;
	}
	return (false);
}

int	ft_isalnum_underscore(int c)
{
	if (ft_isalnum(c))
		return (1);
	if ('_' == c)
		return (1);
	return (0);
}

int	ft_is_singlequote(int c)
{
	if ('\'' == c)
		return (1);
	return (0);
}

int	ft_is_doublequote(int c)
{
	if ('\"' == c)
		return (1);
	return (0);
}

int	ft_is_quote(int c)
{
	if (ft_is_singlequote(c))
		return (1);
	if (ft_is_doublequote(c))
		return (1);
	return (0);
}

