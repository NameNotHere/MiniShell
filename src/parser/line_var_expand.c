/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 18:21:13 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Returns an empty string
TODO: use our custom callo_x instead to catch error
*/
char	*get_empty_string(void)
{
	char	*empty_string;

	empty_string = ft_calloc(1, sizeof(char));
	if (empty_string == NULL)
		return (NULL);
	return (empty_string);
}

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
		if (single_quote && ft_is_singlequote(line[i]))
			single_quote = false;
		else if (single_quote)
			;
		else if (ft_is_singlequote(line[i]))
			single_quote = true;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

int	init_var_expand_arrays(t_msh *sh, t_var_expand *ve)
{
	ft_bzero (ve->var_name_buffer, sizeof(ve->var_name_buffer));
	ve->var_names = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_names)
	{
		printf("allocation error on var names");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_values = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_values)
	{
		printf("allocation error on var values");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	return (EXIT_SUCCESS);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve, char *line)
{
	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	// resets
	ve->var_lookup = false;
	bzero(ve->var_name_buffer, sizeof(ve->var_name_buffer));
	ve->var_name_i = 0;
	return (EXIT_SUCCESS);
}

int	catch_var(t_msh *sh, t_var_expand *ve, char *line, char c)
{
	if (!ft_isalnum_underscore(c))
	{
		ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
		ve->var_values[ve->var_i] = get_empty_string();
		return (catch_absent_var(sh, ve, line));
	}
	ve->var_name_buffer[ve->var_name_i] = c;
	// check if exists now
	// if yes, get the value and its done (run resets)
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	int	i;

	i = 0;
	ve->var_i = -1;
	while (line[i])
	{
		if (ve->var_lookup && catch_var(sh, ve, line, line[i]) != EXIT_SUCCESS)
		{
			printf("error with catch_var on expansion\n");
			return (sh->exit_code);
		}
		if (ve->single_quote && ft_is_singlequote(line[i]))
			ve->single_quote = false;
		else if (ve->single_quote)
			;
		else if (ft_is_singlequote(line[i]))
			ve->single_quote = true;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
		{
			ve->var_i++;
			ve->var_lookup = true;
		}
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
	if (catch_all_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
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
