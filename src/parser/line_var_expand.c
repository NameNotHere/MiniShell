/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/27 21:01:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_parser.h"

int	expand_vars(t_msh *sh, t_var_expand *ve, char *str)
{
	while (str[ve->i])
	{
		if (handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, ve->i))
			;
		else if ('$' == str[ve->i] && ft_valid_var_char(str[ve->i + 1])
			&& !ve->sgl_quote)
		{
			ve->var_lookup = true;
			ve->value = ve->var_values[ve->var_i];
			while (*ve->value)
			{
				ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = *ve->value;
				ve->exp_i++;
				ve->value++;
			}
			ve->i += ft_strlen(ve->var_names[ve->var_i]);
			ve->skipped_chars += ft_strlen(ve->var_names[ve->var_i]) + 1;
			ve->var_i++;
		}
		if (ve->var_lookup == false)
			ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = str[ve->i];
		ve->var_lookup = false;
		ve->i++;
	}
	return (sh->exit_code);
}

void	cleanup_ve(t_var_expand *ve)
{
	safe_free_2d_string(&ve->var_names);
	safe_free_2d_string(&ve->var_values);
	ve->new_str = NULL;
}

/*
 * Expand strings in given string in-place
 * Modifies the string pointer to point to expanded result
 * Returns true on success, false on error
 * Caller must free the result string
 */
bool	expand_string_variables(t_msh *sh, char **string_ptr)
{
	t_var_expand	ve;
	char			*str;

	if (!string_ptr || !*string_ptr)
		return (false);
	str = *string_ptr;
	ft_bzero(&ve, sizeof(t_var_expand));
	ve.str_len = ft_strlen(str);
	if (!ve.str_len)
		return (true);
	ve.var_total = get_var_count(str);
	if (!ve.var_total)
		return (true);
	if (init_var_expand_arrays(sh, &ve) != EXIT_SUCCESS
		|| catch_all_vars(sh, &ve, str) != EXIT_SUCCESS
		|| allocate_new_str(sh, &ve) != EXIT_SUCCESS
		|| expand_vars(sh, &ve, str) != EXIT_SUCCESS)
		return (false);
	free(*string_ptr);
	*string_ptr = ve.new_str;
	cleanup_ve(&ve);
	return (true);
}
