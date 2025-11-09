/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:22:19 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/09 12:22:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Check if string position is outside all quote contexts.
	Returns true when neither single nor double quotes are active.
*/
bool	is_quote_free(t_var_expand *ve)
{
	return !ve->sgl_quote && !ve->dbl_quote;
}

/*
	Checks if variable expansion should occur at given position.
	Returns true if all conditions are met for expansion:
	- Next character is valid variable name char or '?'
	- Not inside single quotes
	- Not inside heredoc delimiter
	- Either PRO mode is off or position is not in skip list
*/
bool	must_expand(t_var_expand *ve, char *str, int pos)
{
	return ((ft_valid_var_char(str[pos + 1]) || str[pos + 1] == '?')
		&& !ve->sgl_quote && !is_in_heredoc_delimiter(str, pos)
		&& (!PRO || !must_skip_exp(ve, pos)));
}

static int	count_operator_chars_in_values(t_var_expand *ve)
{
	int	count;
	int	i;
	int	j;

	count = 0;
	i = 0;
	while (i < ve->var_total && ve->var_values[i])
	{
		j = 0;
		while (ve->var_values[i][j])
		{
			if (is_operator_char(ve->var_values[i][j]))
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

bool	handle_ve_quote(char *str, bool *sgl_quote, bool *dbl_quote, int i)
{
	bool	handled;

	handled = false;
	if (*sgl_quote && is_sgl_quote(str[i]))
	{
		*sgl_quote = false;
		handled = true;
	}
	else if (*dbl_quote && is_dbl_quote(str[i]))
	{
		*dbl_quote = false;
		handled = true;
	}
	else if (!*sgl_quote && !*dbl_quote && is_sgl_quote(str[i]))
	{
		*sgl_quote = true;
		handled = true;
	}
	else if (!*sgl_quote && is_dbl_quote(str[i]))
	{
		*dbl_quote = true;
		handled = true;
	}
	return (handled);
}

bool	handle_sgl_quote(char *str, bool *sgl_quote, int i)
{
	if (*sgl_quote && is_sgl_quote(str[i]))
		*sgl_quote = false;
	else if (*sgl_quote)
		;
	else if (is_sgl_quote(str[i]))
		*sgl_quote = true;
	else
		return (false);
	return (true);
}

int	init_var_expand_arrays(t_msh *sh, t_var_expand *ve)
{
	if (x_calloc_charptr(&ve->var_names, ve->var_total + 1) != EXIT_SUCCESS)
	{
		msg_err(E_INIT_VAR_ARRAYS);
		sh->exit_code = EXIT_FAILURE;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	if (x_calloc_charptr(&ve->var_values, ve->var_total + 1) != EXIT_SUCCESS)
	{
		msg_err(E_INIT_VAR_VALUES);
		sh->exit_code = EXIT_FAILURE;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	reset_var_lookup(ve);
	return (EXIT_SUCCESS);
}

void	reset_var_lookup(t_var_expand *ve)
{
	ve->var_lookup = false;
	bzero(ve->var_name_buffer, sizeof(ve->var_name_buffer));
	ve->var_name_i = 0;
}

/*
	Counts dollar-sign prefixes in var_names.
	For $VAR expansions, var_names stores "VAR" (without $).
	For ~ expansions, var_names stores "~" (includes the tilde itself).
	We need to count how many are $ vars to adjust the size calculation.
*/
static int	count_dollar_vars(t_var_expand *ve)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < ve->var_total && ve->var_names[i])
	{
		if (ve->var_names[i][0] != '~')
			count++;
		i++;
	}
	return (count);
}

int	allocate_new_str(t_msh *sh, t_var_expand *ve)
{
	int	new_str_len;
	int	operator_count;
	int	dollar_vars;

	if (ve->var_total <= 0 || !ve->var_names || !ve->var_values
		|| ve->str_len <= 0)
		return (EXIT_SUCCESS);
	operator_count = count_operator_chars_in_values(ve);
	dollar_vars = count_dollar_vars(ve);
	new_str_len = ve->str_len + ft_strlen_array(ve->var_values)
		- (ft_strlen_array(ve->var_names) + dollar_vars) + operator_count;
	if (x_calloc_char(&ve->new_str, new_str_len + 1))
		return (r_set_exit_perr(sh, E_ALLOC_NEW_STR));
	return (EXIT_SUCCESS);
}
