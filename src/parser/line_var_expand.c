/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/01 01:41:32 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include "minishell.h"

static void	cycle_fix_slash_set_skip(t_var_expand *ve, char *str, char *result)
{
	if (str[ve->i] == '\\' && str[ve->i + 1] == '\\')
	{
		result[(ve->res_i)++] = '\\';
		ve->i += 2;
	}
	else if (str[ve->i] == '\\' && str[ve->i + 1] == '$')
	{
		ve->skipped[ve->skip_len++] = ve->res_i;
		result[(ve->res_i)++] = '$';
		ve->i += 2;
	}
	else
	{
		result[(ve->res_i)++] = str[ve->i];
		(ve->i)++;
	}
}

/*
	Processes backslashes (shortens pairs into literal backslashes)
		and create skip list for escaped variables
	Replaces provided string pointer and updates ve->skipped/ve->skip_len
 */
bool	fix_slashes_set_skips(t_var_expand *ve, char **str_ptr, size_t len)
{
	char	*result;

	if (x_calloc((void **)&result, len + 1, sizeof(char)) != EXIT_SUCCESS
		|| x_calloc((void **)&ve->skipped, len, sizeof(int)) != EXIT_SUCCESS)
	{
		safe_free_string(&result);
		safe_free((void **)&ve->skipped);
		return (false);
	}
	while ((*str_ptr)[ve->i])
		cycle_fix_slash_set_skip(ve, *str_ptr, result);
	ve->i = 0;
	ve->res_i = 0;
	safe_free_string(str_ptr);
	*str_ptr = result;
	return (true);
}

/*
	Check if a given index should be skipped (not expanded)
 */
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

int	expand_vars(t_msh *sh, t_var_expand *ve, char *str)
{
	while (str[ve->i])
	{
		if (handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, ve->i))
			;
		else if ('$' == str[ve->i] && ft_valid_var_char(str[ve->i + 1])
			&& !ve->sgl_quote && !must_skip_exp(ve, ve->i)
			&& !is_in_heredoc_delimiter(str, ve->i))
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

void	cleanup_ve(t_var_expand *ve, bool free_new_str)
{
	safe_free_2d_string(&ve->var_names);
	safe_free_2d_string(&ve->var_values);
	if (ve->skipped)
		free(ve->skipped);
	if (free_new_str)
		safe_free_string(&ve->new_str);
	else
		ve->new_str = NULL;
}

/*
 * Expand strings in given string in-place
 * Modifies the string pointer to point to expanded result
 * Returns true on success, false on error
 * Caller must free the result string
 */
bool	expand_string_variables(t_msh *sh, char **str_ptr)
{
	t_var_expand	ve;

	ft_bzero(&ve, sizeof(t_var_expand));
	if (!fix_slashes_set_skips(&ve, str_ptr, ft_strlen(*str_ptr)))
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
		|| expand_vars(sh, &ve, *str_ptr) != EXIT_SUCCESS)
	{
		safe_free_string(str_ptr);
		cleanup_ve(&ve, true);
		return (false);
	}
	safe_free_string(str_ptr);
	*str_ptr = ve.new_str;
	cleanup_ve(&ve, false);
	return (true);
}
