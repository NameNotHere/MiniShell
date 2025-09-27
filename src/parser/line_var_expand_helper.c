/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:22:19 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 21:30:55 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_quotes_for_expansion(char *line, bool *single_quote, bool *double_quote, int i)
{
	bool	handled;

	handled = false;
	if (*single_quote && ft_is_singlequote(line[i]))
	{
		*single_quote = false;
		handled = true;
	}
	else if (*double_quote && ft_is_doublequote(line[i]))
	{
		*double_quote = false;
		handled = true;
	}
	else if (!*single_quote && !*double_quote && ft_is_singlequote(line[i]))
	{
		*single_quote = true;
		handled = true;
	}
	else if (!*single_quote && ft_is_doublequote(line[i]))
	{
		*double_quote = true;
		handled = true;
	}
	return (handled);
}

bool	handle_single_quote(char *line, bool *single_quote, int i)
{
	if (*single_quote && ft_is_singlequote(line[i]))
		*single_quote = false;
	else if (*single_quote)
		;
	else if (ft_is_singlequote(line[i]))
		*single_quote = true;
	else
		return (false);
	return (true);
}

int	init_var_expand_arrays(t_msh *sh, t_var_expand *ve)
{
	ve->var_names = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_names)
	{
		d_print("allocation error on var names");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_values = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_values)
	{
		d_print("allocation error on var values");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	reset_var_lookup(ve);
	return (EXIT_SUCCESS);
}

void	reset_var_lookup(t_var_expand *ve)
{
	ve->var_lookup = false;
	bzero(ve->var_name_buffer, sizeof(ve->var_name_buffer));
	ve->var_name_i = 0;
}

int	allocate_new_line(t_msh *sh, t_var_expand *ve)
{
	int	new_line_len;

	new_line_len = ve->line_len + ft_strlen_array(ve->var_values)
		- (ft_strlen_array(ve->var_names) + ve->var_total);
	if (callo_x((void **)&ve->newline, new_line_len + 1, sizeof(char))
		!= EXIT_SUCCESS)
	{
		put_stderr("new_line allocation failed\n");
		sh->exit_code = ENOMEM;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	return (EXIT_SUCCESS);
}

void	replace_line_and_cleanup(t_msh *sh, t_var_expand *ve)
{
	safe_free_string(&sh->line);
	sh->line = ve->newline;
	ve->newline = NULL;
	safe_free_2d_string(&ve->var_names);
	safe_free_2d_string(&ve->var_values);
}

