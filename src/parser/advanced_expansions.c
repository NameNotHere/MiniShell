/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_expansions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 01:24:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/* Condition checkers */

/*
	Check if current position has escaped backslash (\\)
	Applies only when PRO
 */
bool	must_fix_escaped_backslash(t_var_expand *ve, char *str)
{
	return (!ve->is_hdoc && !ve->sgl_quote
		&& str[ve->i] == '\\' && str[ve->i + 1] == '\\');
}

/*
	Check if current position has escaped dollar (\$)
	Applies only when PRO
 */
bool	must_fix_escaped_dollar(t_var_expand *ve, char *str)
{
	return (!ve->sgl_quote
		&& str[ve->i] == '\\' && str[ve->i + 1] == '$');
}

/*
	Check if current position has escaped quotes (\", \')
	Applies only when PRO
 */
bool	must_fix_escaped_quotes(t_var_expand *ve, char *str)
{
	return (!ve->sgl_quote && str[ve->i] == '\\'
		&& (str[ve->i + 1] == '"' || str[ve->i + 1] == '\''));
}

/*
	Check if current position has general backslash escape outside quotes
	Applies only when PRO
 */
bool	must_fix_unquoted_backslash(t_var_expand *ve, char *str)
{
	return (!ve->is_hdoc && is_quote_free(ve)
		&& str[ve->i] == '\\' && str[ve->i + 1]);
}

/*
	Check if current position is single quote inside locale syntax ($"...")
	Single quotes need to be replaced with sentinel to preserve them
 */
bool	must_fix_locale_syntax(t_var_expand *ve, char *str)
{
	return (is_quote_free(ve)
		&& str[ve->i] == '$' && str[ve->i + 1] == '"');
}
