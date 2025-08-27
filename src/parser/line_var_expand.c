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
#include "minishell_parser.h"

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

char *join_paths(const char *path1, const char *path2) {
	char	*joined;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(path1);
	len2 = ft_strlen(path2);
	joined = malloc(len1 + len2 + 2);
    if (!joined)
		return NULL;

    ft_strcpy(joined, path1);
    if (len1 > 0 && path1[len1 - 1] != '/')
        ft_strcat(joined, "/");
    return (ft_strcat(joined, path2));
}

char *expand_envp(t_envp *end) {
    char	*prev_expanded;
    char	*full_path;

	if (!end)
        return NULL;
    if (!end->previous) // might not work as intended and do ././ if envp is ./ only
        return strdup(end->folder_name);

    prev_expanded = expand_envp(end->previous);
    full_path = join_paths(prev_expanded, end->folder_name);
    free(prev_expanded);
    return (full_path);
}
