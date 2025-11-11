/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:22:19 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 11:58:27 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Check if string position is outside all quote contexts.
	Returns true when neither single nor double quotes are active.
*/
bool	is_quote_free(t_var_expand *ve)
{
	return (!ve->sgl_quote && !ve->dbl_quote);
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
	return ((is_valid_var_char(str[pos + 1]) || str[pos + 1] == '?')
		&& !ve->sgl_quote && !is_in_heredoc_delimiter(str, pos)
		&& (!PRO || !must_skip_exp(ve, pos)));
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

