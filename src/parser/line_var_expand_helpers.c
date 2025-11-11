/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:12:09 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:36:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Does a few things to support line expansions done correctly:
	1) fixes backslash parsing either inside or outside single quotes (when PRO)
	2) skips expansions on escaped $ char (sets location of skipped expansions)
	3) also skips expansion on use of posix locale syntax ($"..."): $
	char is skipped, variable expansion skipped so what is inside the quotes
	do not expand. note that no translation lookup is supported, so it just
	needs to extract the untranslated content (english basically).

	Proper precedence: The cycle_fix_slash_set_skip function handles escapes
	in the right order (only when PRO):
	1. \\ → \ (prevents false-positive escaped chars)
	2. \$ → $ (with skip marking for variable expansion)
	3. \" / \' → preserve both (for later quote removal)
	4. \X → X (general case, outside double quotes only)

	Single quotes are marked with SGL_QUOTE_MARK to preserve them initially.
	Note: inside $"..." , escapes the single quotes, otherwise they get marked
*/
static void	cycle_pro_substitutions(t_var_expand *ve, char *str, char *result)
{
	if (must_fix_escaped_backslash(ve, str))
		fix_escaped_backslash(ve, result);
	else if (must_fix_escaped_dollar(ve, str))
		fix_escaped_dollar(ve, result);
	else if (must_fix_escaped_quotes(ve, str))
		fix_quoted_chars(ve, str, result);
	else if (must_fix_unquoted_backslash(ve, str))
		fix_unquoted_backslash(ve, str, result);
	else if (must_fix_locale_syntax(ve, str))
		fix_locale_syntax(ve, result, str);
	else if (must_fix_ansi_c_quoting(ve, str))
		fix_ansi_c_quoting(ve, result, str);
	else if (str[ve->i] == '\'')
	{
		result[ve->res_i++] = str[ve->i++];
		ve->sgl_quote = !ve->sgl_quote;
	}
	else if (str[ve->i] == '"')
	{
		result[ve->res_i++] = str[ve->i++];
		ve->dbl_quote = !ve->dbl_quote;
	}
	else
		result[ve->res_i++] = str[ve->i++];
}

/*
	Processes advanced interpreting, expansions and substitutions involving
	backslashes escaped variables or characters, locale syntax and ANSI-C.
	quoting
	Replaces provided string pointer and updates ve->skipped/ve->skip_len.
	Only applies if PRO.
 */
bool	advanced_substitutions(t_var_expand *ve, char **str_ptr)
{
	char	*result;
	size_t	len;

	len = ft_strlen(*str_ptr) + 1;
	if (x_calloc_char(&result, len) != EXIT_SUCCESS
		|| x_calloc_int(&ve->skipped, len) != EXIT_SUCCESS)
	{
		safe_free_str(&result);
		safe_free((void **)&ve->skipped);
		return (false);
	}
	while ((*str_ptr)[ve->i])
		cycle_pro_substitutions(ve, *str_ptr, result);
	ve->i = 0;
	ve->res_i = 0;
	ve->sgl_quote = false;
	ve->dbl_quote = false;
	safe_free_str(str_ptr);
	*str_ptr = result;
	return (true);
}
