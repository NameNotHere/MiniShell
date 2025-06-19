/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isminioperator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:18:52 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/19 02:36:39 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*orhan: added AND / OR cases: && || */
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
