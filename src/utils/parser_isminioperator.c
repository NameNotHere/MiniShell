/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_isminioperator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:18:52 by otanovic          #+#    #+#             */
/*   Updated: 2025/10/28 00:17:30 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

// TODO: rename file to parser_is_operator.c
int	is_operator_continued(char *token, int i);

int	is_operator(char *token, int i)
{
	if (!token)
		return (0);
	if (i > 0 && token[i - 1] == EXP_MARK)
		return (0);
	if (ft_strncmp(token + i, ">>", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "<<", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "&&", 2) == 0)
		return (2);
	if (ft_strncmp(token + i, "||", 2) == 0)
		return (2);
	return (is_operator_continued(token, i));
}

int	is_operator_continued(char *token, int i)
{
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

bool	is_operator_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&'
		|| c == ';' || c == '(' || c == ')');
}
