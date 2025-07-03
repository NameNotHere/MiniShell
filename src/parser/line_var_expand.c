/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 03:59:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	validate_vars(char **line, int var_count, int *var_chars, int *expan_chars)
// {
// 	int		i;
// 	bool	quote;

// 	quote = false;
// 	i = 0;
// 	while (line[i])
// 	{
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

int	count_vars(char *line)
{
	int		var_count;
	int		i;
	bool	quote;

	var_count = 0;
	quote = false;
	i = 0;
	while (line[i])
	{
		if (quote && '\'' == line[i])
			quote = false;
		else if (quote)
			;
		else if ('\'' == line[i])
			quote = true;
		else if ('$' == line[i] && line[i + 1] && '$' != line[i + 1] && \
				ft_isprint(line[i + 1]) && !ft_isspace(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

// int	validate_var(t_msh *sh)
// {
// 	char	buff[1024];
// 	int		i;

// }

int	expand_line(t_msh *sh)
{
	int	line_len;
	int	var_count;
	int	var_chars;
	int	expand_chars;

	line_len = ft_strlen(sh->line);
	if (!line_len)
		return (EXIT_FAILURE);
	// printf("line before expanding is:%s\n", sh->line);
	var_count = count_vars(sh->line);
	// printf("variable count is %d\n", var_count);
	// if (validate_vars(sh->line, var_count, \
	// 	&var_chars, &expand_chars) == EXIT_FAILURE)
	// 	return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
