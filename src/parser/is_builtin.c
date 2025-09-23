/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 13:05:20 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/23 20:33:04 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "cd", 2) == 0 && ft_strlen(str) == 2)
		return (1);
	else if (ft_strncmp(str, "echo", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "export", 6) == 0 && ft_strlen(str) == 6)
		return (1);
	else if (ft_strncmp(str, "unset", 5) == 0 && ft_strlen(str) == 5)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0 && ft_strlen(str) == 3)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0 && ft_strlen(str) == 4)
		return (1);
	return (0);
}
