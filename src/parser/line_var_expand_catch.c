/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:12:44 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/01 02:09:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Check if a position is within a QUOTED heredoc delimiter
	Returns true if the $ at position i is part of a quoted heredoc delimiter
*/
bool	is_in_heredoc_delimiter(char *str, int pos)
{
	int		i;
	int		delimiter_start;
	int		delimiter_end;
	bool	is_quoted;

	i = 0;
	while (str[i])
	{
		if (str[i] == '<' && str[i + 1] == '<')
		{
			i += 2;
			// Skip spaces after <<
			while (str[i] && (str[i] == ' ' || str[i] == '\t'))
				i++;
			delimiter_start = i;
			// Check if delimiter starts with a quote
			is_quoted = (str[i] == '"' || str[i] == '\'');
			// Find end of delimiter (space, tab, newline, or end of string)
			while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
				i++;
			delimiter_end = i;
			// Only skip expansion if delimiter is quoted AND position is within it
			if (is_quoted && pos >= delimiter_start && pos < delimiter_end)
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
	bool	sgl_quote;
	bool	dbl_quote;

	i = 0;
	sgl_quote = false;
	dbl_quote = false;
	var_count = 0;
	while (str[i])
	{
		if (handle_ve_quote(str, &sgl_quote, &dbl_quote, i))
			;
		else if (('$' == str[i] && !must_skip_exp(ve, i))
			&& ft_valid_var_char(str[i + 1]) && !sgl_quote
			&& !is_in_heredoc_delimiter(str, i))
			var_count++;
		i++;
	}
	return (var_count);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve)
{

	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		d_print("allocation error on catch_absent_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	catch_var(t_msh *sh, t_var_expand *ve)
{
	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	if (ve->var_name_buffer[0] == '?')
		ve->var_values[ve->var_i] = ft_itoa(sh->saved_exit_code);
	else
		ve->var_values[ve->var_i] = get_env_value(sh,
				ve->var_name_buffer,
				ve->envp_var_i);
	reset_var_lookup(ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		msg_err("allocation error on catch_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	lookup_var(t_msh *sh, t_var_expand *ve, char c, char next_c)
{
	if (!ft_valid_var_char(c))
		return (catch_absent_var(sh, ve));
	ve->var_name_buffer[ve->var_name_i] = c;
	ve->var_name_i++;
	if (is_var_in_env(sh, ve->var_name_buffer, &ve->envp_var_i))
		return (catch_var(sh, ve));
	else if (next_c == '\0')
		return (catch_absent_var(sh, ve));
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$'
			&& !must_skip_exp(ve, i)
			&& ft_valid_var_char(str[i + 1]) && !ve->sgl_quote
			&& !is_in_heredoc_delimiter(str, i))
		{
			ve->var_lookup = true;
			i++;
		}
		if (ve->var_lookup
			&& lookup_var(sh, ve, str[i], str[i + 1]) != EXIT_SUCCESS)
			return (sh->exit_code);
		handle_ve_quote(str, &ve->sgl_quote, &ve->dbl_quote, i);
		i++;
	}
	ve->var_i = 0;
	ve->var_lookup = false;
	return (EXIT_SUCCESS);
}
