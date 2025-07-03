/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 11:50:08 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_count(char *line)
{
	int		var_count;
	int		i;
	bool	single_quote;


	i = 0;
	single_quote = false;
	var_count = 0;
	while (line[i])
	{
		if (single_quote && '\'' == line[i])
			single_quote = false;
		else if (single_quote)
			;
		else if ('\'' == line[i])
			single_quote = true;
		else if ('$' == line[i] && line[i + 1] && '$' != line[i + 1] && \
				ft_isprint(line[i + 1]) && !ft_isspace(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

int	expand_line(t_msh *sh)
{
	t_var_expand	ve;

	ft_bzero(&ve, sizeof(t_var_expand));
	ve.line_len = ft_strlen(sh->line);
	if (!ve.line_len)
		return (EXIT_SUCCESS); // check later on thiss
	printf("line before expanding is:%s\n", sh->line);
	ve.var_count = get_var_count(sh->line);
	printf("variable count is %d\n", ve.var_count);
	return (EXIT_SUCCESS);
}
