/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isminioperator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:18:52 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/30 13:26:24 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	is_miniop(char *token, int i)
{
	if (ft_strncmp(token + i, "<", 1) == 0)
		return (1);
	if (ft_strncmp(token + i, "|", 1) == 0)
		return (1);
	if (ft_strncmp(token + i, "(", 1) == 0)
		return (1);
	if (ft_strncmp(token + i, ")", 1) == 0)
		return (1);
	if (ft_strncmp(token + i, "&", 1) == 0)
		return (1);
	if (ft_strncmp(token + i, ";", 1) == 0)
		return (1);
	return (0);
}

int	isminioperator(char *token, int i)
{
	if (!token)
		return (0);
	if (ft_strncmp(token + i, ">>", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "<<", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "&&", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "||", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, ">", 1) == 0)
		return (1);
	return (is_miniop(token, i));
}
