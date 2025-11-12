/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_exec.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 13:44:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_expanding_tilde(t_var_expand *ve, char *str)
{
	return (ve->var_names[ve->var_i]
		&& ve->var_names[ve->var_i][0] == '~'
		&& ve->var_names[ve->var_i][1] == '\0'
		&& str[ve->i] == '~');
}

static void	expand_variable_value(t_var_expand *ve)
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

static void	expand_tilde_value(t_var_expand *ve)
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

int	expand_vars(t_var_expand *ve, char *str)
{
	while (str[ve->i])
	{
		if (handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, ve->i))
			;
		else if ('$' == str[ve->i] && must_expand(ve, str, ve->i))
			expand_variable_value(ve);
		else if (is_expanding_tilde(ve, str))
			expand_tilde_value(ve);
		if (ve->var_lookup == false)
			ve->new_str[ve->i + ve->exp_i - ve->skipped_chars] = str[ve->i];
		ve->var_lookup = false;
		ve->i++;
	}
	return (EXIT_SUCCESS);
}
