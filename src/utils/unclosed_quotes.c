/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unclosed_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 00:32:07 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/30 00:34:36 by tda-roch         ###   ########.fr       */
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
	return (single || dbl);
}
