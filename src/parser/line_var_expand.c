/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
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

static bool	is_expanding_tilde(t_var_expand *ve, char *str)
{
	return (ve->var_names[ve->var_i]
		&& ve->var_names[ve->var_i][0] == '~'
		&& ve->var_names[ve->var_i][1] == '\0'
		&& str[ve->i] == '~');
}

int	expand_vars(t_var_expand *ve, char *str)
{
	while (str[ve->i])
	{
		if (handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, ve->i))
			;
		else if ('$' == str[ve->i] && must_expand(ve, str, ve->i))
		{
			ve->var_lookup = true;
			ve->value = ve->var_values[ve->var_i];
			while (*ve->value)
			{
				if (!ve->dbl_quote && is_operator_char(*ve->value))
					ve->new_str[ve->i + ve->exp_i++ - ve->skipped_chars] = EXP_MARK;
				ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = *ve->value;
				ve->exp_i++;
				ve->value++;
			}
			ve->i += ft_strlen(ve->var_names[ve->var_i]);
			ve->skipped_chars += ft_strlen(ve->var_names[ve->var_i]) + 1;
			ve->var_i++;
		}
		else if (is_expanding_tilde(ve, str))
		{
			ve->var_lookup = true;
			ve->value = ve->var_values[ve->var_i];
			while (*ve->value)
			{
				if (is_operator_char(*ve->value))
					ve->new_str[ve->i + ve->exp_i++ - ve->skipped_chars] = EXP_MARK;
				ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = *ve->value;
				ve->exp_i++;
				ve->value++;
			}
			ve->skipped_chars += 1;
			ve->var_i++;
		}
		if (ve->var_lookup == false)
			ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = str[ve->i];
		ve->var_lookup = false;
		ve->i++;
	}
	return (EXIT_SUCCESS);
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
