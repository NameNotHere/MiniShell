/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_expansions_helpers.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:40:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 11:45:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

/*
	Check if current position is ANSI-C quoting syntax ($'...')
	Applies only when PRO
 */
bool	must_fix_ansi_c_quoting(t_var_expand *ve, char *str)
{
	return (is_quote_free(ve)
		&& str[ve->i] == '$' && str[ve->i + 1] == '\'');
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