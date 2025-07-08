/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:12:44 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 19:13:45 by tda-roch         ###   ########.fr       */
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
		if (handle_single_quote(line, &single_quote, i))
			;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve)
{

	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		d_print("allocation error on catch_absent_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	catch_var(t_msh *sh, t_var_expand *ve)
{
	d_print("var %s was caught!\n", ve->var_name_buffer);
	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_env_value(sh, \
									ve->var_name_buffer, \
									ve->envp_var_i);
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		d_print("allocation error on catch_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	d_print("%s=%s\n", ve->var_names[ve->var_i], ve->var_values[ve->var_i]);
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	lookup_var(t_msh *sh, t_var_expand *ve, char c, char next_c)
{
	d_print("lookup_var called with char: '%c'\n", c);
	if (!ft_isalnum_underscore(c))
	{
		d_print("character '%c' is not valid, catching absent var\n", c);
		return (catch_absent_var(sh, ve));
	}
	ve->var_name_buffer[ve->var_name_i] = c;
	ve->var_name_i++;
	d_print("Current var_name_buffer: '%s'\n", ve->var_name_buffer);
	if (is_var_in_env(sh, ve->var_name_buffer, &ve->envp_var_i))
		return (catch_var(sh, ve));
	else if (next_c == '\0')
	{
		d_print("line ended, catching absent var\n");
		return (catch_absent_var(sh, ve));
	}
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ve->var_lookup
			&& lookup_var(sh, ve, line[i], line[i + 1]) != EXIT_SUCCESS)
		{
			d_print("error with catch_var on expansion\n");
			return (sh->exit_code);
		}
		if (handle_single_quote(line, &ve->single_quote, i))
			;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
		{
			d_print("Found $ at position %d, next char: %c\n", i, line[i + 1]);
			ve->var_lookup = true;
		}
		i++;
	}
	ve->var_i = 0;
	ve->var_lookup = false;
	return (EXIT_SUCCESS);
}
