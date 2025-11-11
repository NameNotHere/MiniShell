/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_is_operator.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:18:52 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/10 12:49:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	is_operator_continued(char *token, int i);

int	is_operator(char *token, int i)
{
	if (!token)
		return (EXIT_SUCCESS);
	if (i > 0 && token[i - 1] == EXP_MARK)
		return (EXIT_SUCCESS);
	if (ft_strncmp(token + i, ">>", 2) == 0)
		return (EXIT_SYNTAX);
	if (ft_strncmp(token + i, "<<", 2) == 0)
		return (EXIT_SYNTAX);
	if (ft_strncmp(token + i, ">", 1) == 0)
		return (EXIT_FAILURE);
	if (ft_strncmp(token + i, "<", 1) == 0)
		return (EXIT_FAILURE);
	return (is_operator_continued(token, i));
}

int	is_operator_continued(char *token, int i)
{
	if (VALIDATE && ft_strncmp(token + i, "&&", 2) == 0)
		return (EXIT_SYNTAX);
	if (VALIDATE && ft_strncmp(token + i, "||", 2) == 0)
		return (EXIT_SYNTAX);
	if (VALIDATE && ft_strncmp(token + i, "(", 1) == 0)
		return (EXIT_FAILURE);
	if (VALIDATE && ft_strncmp(token + i, ")", 1) == 0)
		return (EXIT_FAILURE);
	if (VALIDATE && ft_strncmp(token + i, "&", 1) == 0)
		return (EXIT_FAILURE);
	if (VALIDATE && ft_strncmp(token + i, ";", 1) == 0)
		return (EXIT_FAILURE);
	if (ft_strncmp(token + i, "|", 1) == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool	is_operator_char(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '&'
		|| c == ';' || c == '(' || c == ')');
}
