/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 12:37:17 by tda-roch         ###   ########.fr       */
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

int	init_var_expand_arrays(t_msh *sh, t_var_expand *ve)
{
	ve->var_names = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_names)
	{
		printf("allocation error on var names");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_expansions = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_expansions)
	{
		printf("allocation error on var names");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	return (EXIT_SUCCESS);
}

int	catch_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ve->single_quote && '\'' == line[i])
			ve->single_quote = false;
		else if (ve->single_quote)
			if (ve->var_lookup)
				
			;
		else if ('\'' == line[i])
			ve->single_quote = true;
		else if ('$' == line[i] && line[i + 1] && '$' != line[i + 1] && \
				ft_isprint(line[i + 1]) && !ft_isspace(line[i + 1]))
			ve->var_i++;
		i++;
	}
	return (EXIT_SUCCESS);
}

int	expand_line(t_msh *sh)
{
	t_var_expand	ve;

	ft_bzero(&ve, sizeof(t_var_expand));
	ve.line_len = ft_strlen(sh->line);
	if (!ve.line_len)
		return (EXIT_SUCCESS);
	ve.var_total = get_var_count(sh->line);
	if (!ve.var_total)
		return (EXIT_SUCCESS);
	if (init_var_expand_arrays(sh, &ve) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (catch_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
		return (sh->exit_code);
	printf("line before expanding is:%s\n", sh->line);
	printf("variable count is %d\n", ve.var_total);
	// if (expand_vars(sh, ve) != EXIT_SUCCESS)
	// {
	// 	printf("error on variable expansion");
	// 	return (EXIT_FAILURE);
	// }

	return (EXIT_SUCCESS);
}
