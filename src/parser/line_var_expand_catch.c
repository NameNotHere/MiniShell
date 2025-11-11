/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand_catch.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:12:44 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 13:58:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

bool	is_positional_var(t_var_expand *ve, char c);
bool	must_expand_tilde(t_var_expand *ve, char *str, int pos);
int	catch_tilde(t_msh *sh, t_var_expand *ve);

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
		else if (PRO && '~' == str[i] && must_expand_tilde(ve, str, i))
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
		ve->var_values[ve->var_i] = get_env_value_by_idx(sh,
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

