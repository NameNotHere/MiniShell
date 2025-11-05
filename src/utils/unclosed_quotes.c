/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:32:07 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/04 14:34:03 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unclosed_quotes(const char *line)
{
	bool	single;
	bool	dbl;
	int		i;

	single = false;
	dbl = false;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !dbl && !is_escaped(line, i))
			single = !single;
		else if (line[i] == '"' && !single && !is_escaped(line, i))
			dbl = !dbl;
		i++;
	}
	if (single)
		return ('\'');
	if (dbl)
		return ('"');
	return (0);
}

bool	error_unclosed_quotes(const char *line)
{
	int	check_return;

	check_return = unclosed_quotes(line);
	if (check_return == 0)
		return (false);
	if (check_return == '\'')
		msg_err(E_UNCLOSED_SGL_QUOTE);
	else
		msg_err(E_UNCLOSED_DBL_QUOTE);
	return (true);
}
