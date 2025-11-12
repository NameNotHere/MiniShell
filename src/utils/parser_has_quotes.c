/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   has_quotes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 17:01:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

bool	has_quotes(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (true);
		str++;
	}
	return (false);
}
