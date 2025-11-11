/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch_lookup.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:53:49 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lookup_var(t_msh *sh, t_var_expand *ve, char c, char next_c)
{
	if (!ve->var_lookup)
		return (EXIT_SUCCESS);
	if (c == '?')
	{
		ve->var_name_buffer[ve->var_name_i] = c;
		ve->var_name_i++;
		ve->var_name_buffer[ve->var_name_i] = '\0';
		return (catch_var(sh, ve));
	}
	if (!is_valid_var_char(c))
		return (catch_absent_var(sh, ve));
	ve->var_name_buffer[ve->var_name_i] = c;
	ve->var_name_i++;
	ve->var_name_buffer[ve->var_name_i] = '\0';
	if (!is_valid_var_char(next_c) || (PRO && is_positional_var(ve, c)))
	{
		if (is_var_in_env(sh, ve->var_name_buffer, &ve->envp_var_i))
			return (catch_var(sh, ve));
		else
			return (catch_absent_var(sh, ve));
	}
	return (EXIT_SUCCESS);
}

/*
	Captures tilde expansion by storing "~" as name and HOME value.
	Adds to var_names and var_values arrays like regular variables.
*/
int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && must_expand(ve, str, i))
		{
			ve->var_lookup = true;
			i++;
		}
		else if (PRO && str[i] == '~' && must_expand_tilde(ve, str, i))
		{
			if (catch_tilde(sh, ve) != EXIT_SUCCESS)
				return (sh->exit_code);
		}
		if (lookup_var(sh, ve, str[i], str[i + 1]) != EXIT_SUCCESS)
			return (sh->exit_code);
		handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, i);
		i++;
	}
	ve->var_i = 0;
	ve->var_lookup = false;
	return (EXIT_SUCCESS);
}
