/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch_helpers.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:49:28 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 13:58:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Checks if this is a positional variable ($0-$9).
	Positional variables are single digits only; $10+ require braces.
	Assumes caller has already checked PRO mode.
*/
bool	is_positional_var(t_var_expand *ve, char c)
{
	return (ve->var_name_i == 1 && ft_isdigit(c));
}

/*
	Checks if tilde should be expanded at given position.
	Tilde expansion rules (matching bash behavior):
	- Must not be inside single or double quotes
	- Must be at start of word (after space/tab)
	  Examples: echo ~, cd ~/test
	- If followed by a character, must be / or whitespace or operator
	  This means: ~ and ~/... expand, but ~user does NOT (not implemented)
	- End of string is valid: echo ~ (expands)

	Note: Bash also expands after : and = in assignment context (VAR=~),
	but minishell doesn't handle variable assignments so we skip those.

	Note: Caller must check PRO mode before calling.
*/
bool	must_expand_tilde(t_var_expand *ve, char *str, int pos)
{
	if (ve->sgl_quote || ve->dbl_quote)
		return (false);
	if (pos > 0 && str[pos - 1] != ' ' && str[pos - 1] != '\t')
		return (false);
	if (str[pos + 1] && str[pos + 1] != '/' && str[pos + 1] != ' '
		&& str[pos + 1] != '\t' && str[pos + 1] != ':'
		&& str[pos + 1] != '|' && str[pos + 1] != '>' && str[pos + 1] != '<'
		&& str[pos + 1] != '&' && str[pos + 1] != ';')
		return (false);
	return (true);
}

/*
	Captures tilde expansion by storing "~" as name and HOME value.
	Adds to var_names and var_values arrays like regular variables.
*/
int	catch_tilde(t_msh *sh, t_var_expand *ve)
{
	ve->var_names[ve->var_i] = ft_strdup("~");
	ve->var_values[ve->var_i] = get_env_value_by_name(sh, "HOME");
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		msg_err("tilde expansion failed");
		sh->exit_code = EXIT_FAILURE;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}
