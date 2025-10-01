/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:22:19 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 23:39:20 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_ve_quote(char *str, bool *sgl_quote, bool *dbl_quote, int i)
{
	bool	handled;

	handled = false;
	if (*sgl_quote && is_sgl_quote(str[i]))
	{
		*sgl_quote = false;
		handled = true;
	}
	else if (*dbl_quote && is_dbl_quote(str[i]))
	{
		*dbl_quote = false;
		handled = true;
	}
	else if (!*sgl_quote && !*dbl_quote && is_sgl_quote(str[i]))
	{
		*sgl_quote = true;
		handled = true;
	}
	else if (!*sgl_quote && is_dbl_quote(str[i]))
	{
		*dbl_quote = true;
		handled = true;
	}
	return (handled);
}

bool	handle_sgl_quote(char *str, bool *sgl_quote, int i)
{
	if (*sgl_quote && is_sgl_quote(str[i]))
		*sgl_quote = false;
	else if (*sgl_quote)
		;
	else if (is_sgl_quote(str[i]))
		*sgl_quote = true;
	else
		return (false);
	return (true);
}

// TODO: remove d_prints, add actuall error messages
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

int	allocate_new_str(t_msh *sh, t_var_expand *ve)
{
	int	new_str_len;

	if (ve->var_total <= 0 || !ve->var_names || !ve->var_values || ve->str_len <= 0)
		return (EXIT_SUCCESS);
	new_str_len = ve->str_len + ft_strlen_array(ve->var_values)
		- (ft_strlen_array(ve->var_names) + ve->var_total);
	if (x_calloc((void **)&ve->new_str, new_str_len + 1, sizeof(char))
		!= EXIT_SUCCESS)
	{
		put_stderr("new_str allocation failed\n");
		sh->exit_code = ENOMEM;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	return (EXIT_SUCCESS);
}

