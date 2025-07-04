/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/04 16:35:07 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_array(char **array)
{
	int	count;

	count = 0;
	while (*array)
	{
		count += ft_strlen(*array);
		array++;
	}
	return (count);
}

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
			&& sh->envp[i][var_len] == '=')
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

	var_value = ft_strdup(sh->envp[ve->envp_var_i] + ft_strlen(var) + 1);
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
		if (handle_single_quote(line, &single_quote, i))
			;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
			var_count++;
		i++;
	}
	return (var_count);
}

void	reset_var_lookup(t_msh *sh, t_var_expand *ve)
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
	reset_var_lookup(sh, ve);
	return (EXIT_SUCCESS);
}

int	catch_absent_var(t_msh *sh, t_var_expand *ve)
{

	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_empty_string();
	reset_var_lookup(sh, ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		printf("allocation error on catch_absent_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int catch_var(t_msh *sh, t_var_expand *ve)
{
	printf("var %s was caught!\n", ve->var_name_buffer);
	ve->var_names[ve->var_i] = ft_strdup(ve->var_name_buffer);
	ve->var_values[ve->var_i] = get_var_value(sh, ve, ve->var_name_buffer);
	reset_var_lookup(sh, ve);
	if (!ve->var_names[ve->var_i] || !ve->var_values[ve->var_i])
	{
		printf("allocation error on catch_var\n");
		sh->exit_code = errno;
		return (errno);
	}
	printf("%s=%s\n", ve->var_names[ve->var_i], ve->var_values[ve->var_i]);
	ve->var_i++;
	return (EXIT_SUCCESS);
}

int	lookup_var(t_msh *sh, t_var_expand *ve, char c, char next_c)
{
	printf("lookup_var called with char: '%c'\n", c);
	if (!ft_isalnum_underscore(c))
	{
		printf("character '%c' is not valid, catching absent var\n", c);
		return (catch_absent_var(sh, ve));
	}
	ve->var_name_buffer[ve->var_name_i] = c;
	ve->var_name_i++;
	printf("Current var_name_buffer: '%s'\n", ve->var_name_buffer);
	if (is_var_in_env(sh, ve, ve->var_name_buffer))
		return (catch_var(sh, ve));
	else if (next_c == '\0')
	{
		printf("line ended, catching absent var\n");
		return (catch_absent_var(sh, ve));
	}
	return (EXIT_SUCCESS);
}

int	catch_all_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (ve->var_lookup
			&& lookup_var(sh, ve, line[i], line[i + 1]) != EXIT_SUCCESS)
		{
			printf("error with catch_var on expansion\n");
			return (sh->exit_code);
		}
		if (handle_single_quote(line, &ve->single_quote, i))
			;
		else if ('$' == line[i] && ft_isalnum_underscore(line[i + 1]))
		{
			printf("Found $ at position %d, next char: %c\n", i, line[i + 1]);
			ve->var_lookup = true;
		}
		i++;
	}
	ve->var_i = 0;
	ve->var_lookup = false;
	return (EXIT_SUCCESS);
}

int	expand_vars(t_msh *sh, t_var_expand *ve, char *line)
{
	while (line[ve->i])
	{
		if (handle_single_quote(line, &ve->single_quote, ve->i))
			;
		else if ('$' == line[ve->i] && ft_isalnum_underscore(line[ve->i + 1]))
		{
			ve->var_lookup = true;
			ve->value = ve->var_values[ve->var_i];
			while (*ve->value)
			{
				ve->newline[ve->i + ve->exp_i - ve->skipped_chars] = *ve->value;
				ve->exp_i++;
				ve->value++;
			}
			ve->i += ft_strlen(ve->var_names[ve->var_i]);
			ve->skipped_chars += ft_strlen(ve->var_names[ve->var_i]) + 1;
			ve->var_i++;
		}
		if (ve->var_lookup == false)
			ve->newline[ve->i + ve->exp_i - ve->skipped_chars] = line[ve->i];
		ve->var_lookup = false;
		ve->i++;
	}
	return (sh->exit_code);
}

int	allocate_new_line(t_msh *sh, t_var_expand *ve)
{
	int	new_line_len;

	new_line_len = ve->line_len + ft_strlen_array(ve->var_values)
		- (ft_strlen_array(ve->var_names) + ve->var_total);
	if (callo_x((void **)&ve->newline, new_line_len + 1, sizeof(char))
		!= EXIT_SUCCESS)
	{
		printf("new_line allocation failed\n");
		sh->exit_code = ENOMEM;
		if (errno)
			sh->exit_code = errno;
		return (sh->exit_code);
	}
	printf("line allocated!\n");
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
	printf("line before expanding is:%s\n", sh->line);
	printf("variable count is %d\n", ve.var_total);
	if (catch_all_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
		return (sh->exit_code);
	printf("variable names size: %d\n", ft_strlen_array(ve.var_names));
	printf("variable values size: %d\n", ft_strlen_array(ve.var_values));
	if (allocate_new_line(sh, &ve) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (expand_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
	{
		printf("error on variable expansion");
		printf("this function should not fail though\n");
		sh->exit_code = EXIT_FAILURE;
		return (sh->exit_code);
	}
	printf("new_line made: %s\n", ve.newline);
	safe_free_string(&sh->line);
	sh->line = ve.newline;
	ve.newline = NULL;
	safe_free_2d_string(&ve.var_names);
	safe_free_2d_string(&ve.var_values);
	return (EXIT_SUCCESS);
}
