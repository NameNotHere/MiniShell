/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_expansions.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 01:24:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 11:39:23 by tda-roch         ###   ########.fr       */
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

/*
	Check if current position is ANSI-C quoting syntax ($'...')
	Applies only when PRO
 */
bool	must_fix_ansi_c_quoting(t_var_expand *ve, char *str)
{
	return (is_quote_free(ve)
		&& str[ve->i] == '$' && str[ve->i + 1] == '\'');
}

/* Action functions */

/*
	Fix escaped backslash: \\ → \
	Applies only when PRO
 */
void	fix_escaped_backslash(t_var_expand *ve, char *result)
{
	result[(ve->res_i)++] = '\\';
	ve->i += 2;
}

/*
	Fix escaped dollar: \$ → $
	Mark position in skip list to prevent variable expansion
	Applies only when PRO
 */
void	fix_escaped_dollar(t_var_expand *ve, char *result)
{
	ve->skipped[ve->skip_len++] = ve->res_i;
	result[(ve->res_i)++] = '$';
	ve->i += 2;
}

/*
	Fix escaped quotes: \" and \' → preserve both
	Applies only when PRO
 */
void	fix_quoted_chars(t_var_expand *ve, char *str, char *result)
{
	result[(ve->res_i)++] = '\\';
	result[(ve->res_i)++] = str[ve->i + 1];
	ve->i += 2;
}

/*
	Fix general backslash escape outside quotes: \X → X
	Applies only when PRO
 */
void	fix_unquoted_backslash(t_var_expand *ve, char *str, char *result)
{
	result[(ve->res_i)++] = str[ve->i + 1];
	ve->i += 2;
}

/*
	Fix single quotes inside locale syntax ($"...")
	Replace single quotes with ESCAPED_SGL_QUOTE sentinel to preserve them
	Mark position in skip list to prevent variable expansion
	Content inside $"..." is not expanded for variables
	Applies only when PRO
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
	Handle ANSI-C escape sequences within $'...' quoting.

	Supported escape sequences:
	- \n : newline (LF, ASCII 10)
	- \t : horizontal tab (ASCII 9)
	- \r : carriage return (CR, ASCII 13)
	- \\ : backslash
	- \' : single quote

	Not supported (kept as-is for unknown escapes):
	- \xHH : hex escapes
	- \OOO : octal escapes
	- \a, \b, \f, \v : alert/backspace/form-feed/vertical-tab
	- \uHHHH, \UHHHHHHHH : unicode escapes
	- \" : double quote (rarely needed in single-quoted context)
 */
static void	fix_ansi_c_escapes(t_var_expand *ve, char *result, char *str)
{
	if (str[ve->i + 1] == 'n')
		result[ve->res_i++] = '\n';
	else if (str[ve->i + 1] == 't')
		result[ve->res_i++] = '\t';
	else if (str[ve->i + 1] == 'r')
		result[ve->res_i++] = '\r';
	else if (str[ve->i + 1] == '\\')
		result[ve->res_i++] = '\\';
	else if (str[ve->i + 1] == '\'')
		result[ve->res_i++] = '\'';
	else
	{
		result[ve->res_i++] = str[ve->i];
		result[ve->res_i++] = str[ve->i + 1];
	}
	ve->i += 2;
}

/*
	Process ANSI-C quoting syntax ($'...')

	Mark position in skip list to prevent variable expansion.
	Content inside $'...' is not expanded for variables.
	Applies only when PRO.
 */
void	fix_ansi_c_quoting(t_var_expand *ve, char *result, char *str)
{
	ve->skipped[ve->skip_len++] = ve->res_i;
	ve->i += 2;
	while (str[ve->i] && str[ve->i] != '\'')
	{
		if (str[ve->i] == '\\' && str[ve->i + 1])
			fix_ansi_c_escapes(ve, result, str);
		else
			result[ve->res_i++] = str[ve->i++];
	}
	if (str[ve->i] == '\'')
		ve->i++;
}
