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

	single = false;
	dbl = false;
	while (*line)
	{
		if (*line == '\'' && !dbl)
			single = !single;
		else if (*line == '"' && !single)
			dbl = !dbl;
		line++;
	}
	return (single || dbl);
}
