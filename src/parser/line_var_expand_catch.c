/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:12:44 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/06 19:46:14 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Checks if this is a positional variable ($0-$9).
	Positional variables are single digits only; $10+ require braces.
	Assumes caller has already checked PRO mode.
*/
static bool	is_positional_var(t_var_expand *ve, char c)
{
	return (ve->var_name_i == 1 && ft_isdigit(c));
}


bool	is_in_heredoc_delimiter(char *str, int pos)
{
	int		i;
	int		delimiter_start;
	int		delimiter_end;

	i = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i += 2;
			while (str[i] && ft_isspace(str[i]))
				i++;
			delimiter_start = i;
			while (str[i] && !ft_isspace(str[i]))
				i++;
			delimiter_end = i;
			if ((str[delimiter_start] == '"' || str[delimiter_start] == '\'')
				&& pos >= delimiter_start && pos < delimiter_end)
				return (true);
		}
		else
			i++;
	}
	return (false);
}

int	get_var_count(char *str, t_var_expand *ve)
{
	int		var_count;
	int		i;

	i = 0;
	var_count = 0;
	while (str[i])
	{
		if (handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, i))
			;
		else if ('$' == str[i] && must_expand(ve, str, i))
			var_count++;
		i++;
	}
	ve->sgl_quote = false;
	ve->dbl_quote = false;
	return (var_count);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve)
{

	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		msg_err(E_CATCH_ABSENT_VAR);
		sh->exit_code = EXIT_FAILURE;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	catch_var(t_msh *sh, t_var_expand *ve)
{
	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	if (ve->var_name_buffer[0] == '?')
	{
		ve->var_values[ve->var_i] = ft_itoa(sh->saved_exit_code);
		if (!ve->var_values[ve->var_i])
		{
			msg_err(E_ITOA_FAILED);
			safe_free_str(&ve->var_names[ve->var_i]);
			sh->exit_code = errno;
			return (errno);
		}
	}
	else
		ve->var_values[ve->var_i] = get_env_value(sh,
				ve->var_name_buffer,
				ve->envp_var_i);
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		msg_err(E_CATCH_VAR);
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	lookup_var(t_msh *sh, t_var_expand *ve, char c, char next_c)
{
	if (!ve->var_lookup)
		return (EXIT_SUCCESS);
	if (c == '?')
	{
		ve->var_name_buffer[ve->var_name_i] = c;
		ve->var_name_i++;
		ve->var_name_buffer[ve->var_name_i] = '\0';
		return (catch_var(sh, ve));
	}
	if (!ft_valid_var_char(c))
		return (catch_absent_var(sh, ve));
	ve->var_name_buffer[ve->var_name_i] = c;
	ve->var_name_i++;
	ve->var_name_buffer[ve->var_name_i] = '\0';
	if (!ft_valid_var_char(next_c) || (PRO && is_positional_var(ve, c)))
	{
		if (is_var_in_env(sh, ve->var_name_buffer, &ve->envp_var_i))
			return (catch_var(sh, ve));
		else
			return (catch_absent_var(sh, ve));
	}
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && must_expand(ve, str, i))
		{
			ve->var_lookup = true;
			i++;
		}
		if (lookup_var(sh, ve, str[i], str[i + 1]) != EXIT_SUCCESS)
			return (sh->exit_code);
		handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, i);
		i++;
	}
	ve->var_i = 0;
	ve->var_lookup = false;
	return (EXIT_SUCCESS);
}
