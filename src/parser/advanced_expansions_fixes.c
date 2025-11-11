/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced_expansions_fixes.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:40:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 11:45:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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