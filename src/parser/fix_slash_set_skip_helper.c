/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_slash_set_skip_helper.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 01:24:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 03:38:42 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/* Condition checkers */

/*
 * Check if current position has escaped backslash (\\)
 * Applies only when PRO=1
 */
bool	must_fix_escaped_backslash(t_var_expand *ve, char *str)
{
	return (PRO && !ve->is_hdoc && !ve->sgl_quote
		&& str[ve->i] == '\\' && str[ve->i + 1] == '\\');
}

/*
 * Check if current position has escaped dollar (\$)
 * Applies only when PRO=1
 */
bool	must_fix_escaped_dollar(t_var_expand *ve, char *str)
{
	return (PRO && !ve->sgl_quote
		&& str[ve->i] == '\\' && str[ve->i + 1] == '$');
}

/*
	Check if current position has escaped quotes (\", \')
	Applies only when PRO=1
 */
bool	must_fix_escaped_quotes(t_var_expand *ve, char *str)
{
	return (PRO && !ve->sgl_quote
		&& str[ve->i] == '\\'
		&& (str[ve->i + 1] == '"' || str[ve->i + 1] == '\''));
}

/*
	Check if current position has general backslash escape outside quotes
	Applies only when PRO=1
 */
bool	must_fix_unquoted_backslash(t_var_expand *ve, char *str)
{
	return (PRO && !ve->is_hdoc && !ve->sgl_quote && !ve->dbl_quote
		&& str[ve->i] == '\\' && str[ve->i + 1]);
}

/*
	Check if current position is single quote inside locale syntax ($"...")
	Single quotes need to be replaced with sentinel to preserve them
 */
bool	must_fix_locale_syntax(t_var_expand *ve, char *str)
{
	return (PRO && !ve->sgl_quote && !ve->dbl_quote
		&& str[ve->i] == '$' && str[ve->i + 1] == '"');
}

/*
	Check if current position is ANSI-C quoting syntax ($'...')
	Applies only when PRO=1
 */
bool	must_fix_ansi_c_quoting(t_var_expand *ve, char *str)
{
	return (PRO && !ve->sgl_quote && !ve->dbl_quote
		&& str[ve->i] == '$' && str[ve->i + 1] == '\'');
}

/* Action functions */

/*
	Fix escaped backslash: \\ → \
	Applies only when PRO=1
 */
void	fix_escaped_bkslash(t_var_expand *ve, char *result)
{
	result[(ve->res_i)++] = '\\';
	ve->i += 2;
}

/*
	Fix escaped dollar: \$ → $
	Mark position in skip list to prevent variable expansion
	Applies only when PRO=1
 */
void	fix_escaped_dollar(t_var_expand *ve, char *result)
{
	ve->skipped[ve->skip_len++] = ve->res_i;
	result[(ve->res_i)++] = '$';
	ve->i += 2;
}

/*
	Fix escaped quotes: \" and \' → preserve both
	Applies only when PRO=1
 */
void	fix_quoted_chars(t_var_expand *ve, char *str, char *result)
{
	result[(ve->res_i)++] = '\\';
	result[(ve->res_i)++] = str[ve->i + 1];
	ve->i += 2;
}

/*
	Fix general backslash escape outside quotes: \X → X
	Applies only when PRO=1
 */
void	fix_unquoted_bkslash(t_var_expand *ve, char *str, char *result)
{
	result[(ve->res_i)++] = str[ve->i + 1];
	ve->i += 2;
}

/*
	Fix single quotes inside locale syntax ($"...")
	Replace single quotes with ESCAPED_SGL_QUOTE sentinel to preserve them
	Mark position in skip list to prevent variable expansion
	Content inside $"..." is not expanded for variables
	Applies only when PRO=1
 */
void	fix_locale_syntax(t_var_expand *ve, char *result, char *str)
{
	ve->skipped[ve->skip_len++] = ve->res_i;
	ve->i += 2;
	while (str[ve->i] && str[ve->i] != '"')
	{
		if (str[ve->i] == '\'')
			result[(ve->res_i)++] = ESCAPED_SGL_QUOTE;
		else
			result[(ve->res_i)++] = str[ve->i];
		ve->i++;
	}
	if (str[ve->i] == '"')
		ve->i++;
}

/*
	Fix ANSI-C quoting syntax ($'...')
	Mark position in skip list to prevent variable expansion
	Content inside $'...' is not expanded for variables
	Applies only when PRO=1
 */
void	fix_ansi_c_quoting(t_var_expand *ve, char *result, char *str)
{
	ve->skipped[ve->skip_len++] = ve->res_i;
	ve->i += 2;
	while (str[ve->i] && str[ve->i] != '\'')
		result[(ve->res_i)++] = str[ve->i++];
	if (str[ve->i] == '\'')
		ve->i++;
}
