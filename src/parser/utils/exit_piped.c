/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_piped.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:49:57 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/01 17:19:38 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

/*
helper function for minishell_mainloop, to process an early exit.
checks that string has not a pipe. if it does, exit command needs to be
processed during ast processing (it will not exit minishell itself).
*/
bool	exit_piped(char *line)
{
	// int		i;
	// bool	in_squote;
	// bool	in_dquote;
	// i = 0;
	// in_squote = false;
	// in_dquote = false;
	/*this is a mess -> rewriting */
	// while (line[i])
	// {
	// 	if (!in_squote && !in_dquote)
	// 	{
	// 		if (line[i] == '\'')
	// 			in_squote = true;
	// 		else if (line[i] == '\"')
	// 			in_dquote = true;
	// 		if (in_squote || in_dquote)
	// 			i++;
	// 	}
	// 	while ((in_squote && line[i] != '\'') || (in_dquote && line[i] != '\"'))
	// 		i++;
	// 	in_dquote = false;
	// 	in_squote = false;
	// 	while (ft_strchr("|\'\"", line[i]) && line[i] != '\0')
	// 		i++;
	// }
	return (false);
}

