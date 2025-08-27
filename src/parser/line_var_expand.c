/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_var_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:07:42 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/07 19:14:02 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	d_print("new_line made: %s\n", ve->newline);
	return (sh->exit_code);
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
	d_print("line before expanding is:%s\n", sh->line);
	if (catch_all_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (allocate_new_line(sh, &ve) != EXIT_SUCCESS)
		return (sh->exit_code);
	if (expand_vars(sh, &ve, sh->line) != EXIT_SUCCESS)
	{
		d_print("error on variable expansion");
		sh->exit_code = EXIT_FAILURE;
		return (sh->exit_code);
	}
	replace_line_and_cleanup(sh, &ve);
	return (EXIT_SUCCESS);
}

char	*expand_envp(s_envp end)
{
	//add to the end all of end->prev recursivly
	char	*ret;

	if (!end.previous)
		return (end.folder_name);

	end.folder_name = ft_strlcat(end.previous->folder_name, end.folder_name, SIZE);
	end.previous = end.previous->previous; 
	end.folder_name = expand_envp(end);
	return (end.folder_name);
	// returns this plus the previous folder name and calls this function with the new input this input but changed folder name
}