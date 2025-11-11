/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	must_skip_exp(t_var_expand *ve, int index)
{
	int	i;

	i = 0;
	while (i < ve->skip_len)
	{
		if (ve->skipped[i] == index)
			return (true);
		i++;
	}
	return (false);
}

void	cleanup_ve(t_var_expand *ve, bool free_new_str)
{
	safe_free_2d_string(&ve->var_names);
	safe_free_2d_string(&ve->var_values);
	if (ve->skipped)
		free(ve->skipped);
	if (free_new_str)
		safe_free_str(&ve->new_str);
	else
		ve->new_str = NULL;
}

/*
 * Expand strings in given string in-place
 * Modifies the string pointer to point to expanded result
 * Returns true on success, false on error
 * Caller must free the result string
 */
bool	expand_string_variables(t_msh *sh, char **str_ptr, bool is_hdoc)
{
	t_var_expand	ve;
	bool			success;

	success = true;
	ft_bzero(&ve, sizeof(t_var_expand));
	ve.is_hdoc = is_hdoc;
	if (PRO && !advanced_substitutions(&ve, str_ptr))
		return (false);
	ve.str_len = ft_strlen(*str_ptr);
	if (!ve.str_len)
		return (safe_free((void **)&ve.skipped), true);
	ve.var_total = get_var_count(*str_ptr, &ve);
	if (!ve.var_total)
		return (safe_free((void **)&ve.skipped), true);
	if (init_var_expand_arrays(sh, &ve) != EXIT_SUCCESS
		|| catch_all_vars(sh, &ve, *str_ptr) != EXIT_SUCCESS
		|| allocate_new_str(sh, &ve) != EXIT_SUCCESS
		|| expand_vars(&ve, *str_ptr) != EXIT_SUCCESS)
		success = false;
	safe_free_str(str_ptr);
	if (success)
		*str_ptr = ve.new_str;
	cleanup_ve(&ve, !success);
	return (success);
}
