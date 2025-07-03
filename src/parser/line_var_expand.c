/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/03 22:42:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*if true, sets also the env_var_location, to find the var index in envp*/
bool	is_var_in_env(t_msh *sh, t_var_expand *ve, char *var)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (sh->envp[i])
	{
		if ((ft_strncmp(sh->envp[i], var, var_len) == 0)
			&& sh->envp[i][var_len + 1] == '=')
		{
			ve->envp_var_i = i;
			return (true);
		}
		i++;
	}
	return (false);
}

char	*get_var_value(t_msh *sh, t_var_expand *ve, char *var)
{
	char	*var_value;

	var_value = ft_strdup(sh->envp[ve->envp_var_i] + ft_strlen(var));
	if (!var_value)
	{
		sh->exit_code = errno;
		return (NULL);
	}
	return (var_value);
}

/*Returns an empty string
TODO: use our custom callo_x instead to catch error
*/
char	*get_empty_string(void)
{
	char	*empty_string;

	empty_string = ft_calloc(1, sizeof(char));
	if (empty_string == NULL)
		return (NULL);
	return (empty_string);
}

int	get_var_count(char *line)
{
	int		var_count;
	int		i;
	bool	single_quote;

	i = 0;
	single_quote = false;
	var_count = 0;
	while (line[i])
	{
		if (single_quote && ft_is_singlequote(line[i]))
			single_quote = false;
		else if (single_quote)
			;
		else if (ft_is_singlequote(line[i]))
			single_quote = true;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

void	reset_var_expand(t_msh *sh, t_var_expand *ve)
{
	(void)sh;
	ve->var_lookup = false;
	bzero(ve->var_name_buffer, sizeof(ve->var_name_buffer));
	ve->var_name_i = 0;
}

int	init_var_expand_arrays(t_msh *sh, t_var_expand *ve)
{
	ve->var_names = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_names)
	{
		printf("allocation error on var names");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_values = ft_calloc((ve->var_total + 1), sizeof(char *));
	if (!ve->var_values)
	{
		printf("allocation error on var values");
		sh->err = errno;
		sh->exit_code = errno;
		return (errno);
	}
	return (EXIT_SUCCESS);
	reset_var_expand(sh, ve);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve)
{

	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	reset_var_expand(sh, ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		printf("allocation error on absent var\n");
		sh->exit_code = errno;
		return (errno);
	}
	return (EXIT_SUCCESS);
}

int	catch_var(t_msh *sh, t_var_expand *ve, char c)
{
	if (!ft_isalnum_underscore(c))
	{
		ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
		ve->var_values[ve->var_i] = get_empty_string();
		return (catch_absent_var(sh, ve));
	}
	ve->var_name_buffer[ve->var_name_i] = c;
	if (is_var_in_env(sh, ve, ve->var_name_buffer))
	{
		ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
		ve->var_values[ve->var_i] = get_var_value(sh, ve, ve->var_name_buffer);
		if (!ve->var_values[ve->var_i])
			return (sh->exit_code);
	}
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	int	i;

	i = 0;
	ve->var_i = -1;
	while (line[i])
	{
		if (ve->var_lookup && catch_var(sh, ve, line[i]) != EXIT_SUCCESS)
		{
			printf("error with catch_var on expansion\n");
			return (sh->exit_code);
		}
		if (ve->single_quote && ft_is_singlequote(line[i]))
			ve->single_quote = false;
		else if (ve->single_quote)
			;
		else if (ft_is_singlequote(line[i]))
			ve->single_quote = true;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
		{
			ve->var_i++;
			ve->var_lookup = true;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	expand_line(t_msh *sh)
{
	t_var_expand	ve;

	ft_bzero(&ve, sizeof(t_var_expand));
	ve.line_len = ft_strlen(sh->line);
	if (!ve.line_len)
		return (EXIT_SUCCESS);
	ve.var_total = get_var_count(sh->line);
	if (!ve.var_total)
		return (EXIT_SUCCESS);
	if (init_var_expand_arrays(sh, &ve) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (catch_all_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
		return (sh->exit_code);
	printf("line before expanding is:%s\n", sh->line);
	printf("variable count is %d\n", ve.var_total);
	// if (expand_vars(sh, ve) != EXIT_SUCCESS)
	// {
	// 	printf("error on variable expansion");
	// 	return (EXIT_FAILURE);
	// }
	return (EXIT_SUCCESS);
}
