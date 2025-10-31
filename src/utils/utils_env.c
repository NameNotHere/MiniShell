/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:28:25 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/29 19:43:08 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

/*
Returns true if variable string found in envp or var is $?,
	- If a non-null int pointer is passed:
		also sets it to the index of the var in in envp

Usage:
	if the index is not needed, just pass a NULL to envp_index.
	If the index is needed, pass an int by address, it will update to value.
*/
bool	is_var_in_env(t_msh *sh, char *var, int *envp_index)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	if (var_len && var[0] == '?')
		return (true);
	while (sh->envp[i])
	{
		if ((ft_strncmp(sh->envp[i], var, var_len) == 0)
			&& sh->envp[i][var_len] == '=')
		{
			if (envp_index)
				*envp_index = i;
			return (true);
		}
		i++;
	}
	return (false);
}

/*
Returns value from an environment variable
Needs both the var name and the index.

TODO: probably redundant to pass both...
TODO: check maybe separate in two kinds of lookup (by name / by index)

*/
char	*get_env_value(t_msh *sh, char *var_name, int envp_index)
{
	char	*var_value;

	if (envp_index < 0 || !sh->envp[envp_index])
		return (ft_strdup(""));
	var_value = ft_strdup(sh->envp[envp_index] + ft_strlen(var_name) + 1);
	if (!var_value)
	{
		set_exit_perr(sh, ERRNO_CODE, "minishell: get_env_value allocation failed\n");
		return (NULL);
	}
	return (var_value);
}

// updates shell level variable. safe to ignore return,
// not critical nor required for minishell.
int	update_shell_level_var(t_msh *sh)
{
	char	*assign;
	char	*var_value;
	char	*equals_pos;
	int		var_int;
	int		var_index;

	assign = NULL;
	var_index = search_name("SHLVL", sh->envp);
	if (var_index == -1)
		assign = ft_strdup("SHLVL=1");
	else
	{
		var_value = get_env_value(sh, "SHLVL", var_index);
		var_int = min_int(max_int(ft_atoi(var_value), 0), INT_MAX - 1) + 1;
		safe_free_string(&var_value);
		var_value = ft_itoa(var_int);
		assign = ft_strjoin("SHLVL=", var_value);
		safe_free_string(&var_value);
	}
	if (!assign)
		return (EXIT_FAILURE);
	equals_pos = ft_strchr(assign, '=');
	handle_export_assignment(&sh, assign, equals_pos);
	safe_free_string(&assign);
	return (EXIT_SUCCESS);
}

int	initialize_null_env(t_msh *sh)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ret_exit_perr(sh, ERRNO_CODE, E_INIT_ENV_MSG));
	sh->envp = ft_calloc(5, sizeof(char *));
	if (!sh->envp)
		return (safe_free_string(&cwd),
			ret_exit_perr(sh, ERRNO_CODE, E_INIT_ENV_MSG));
	sh->envp[0] = ft_strjoin("PWD=", cwd);
	sh->envp[1] = ft_strjoin("OLDPWD=", cwd);
	sh->envp[2] = ft_strdup("SHLVL=");
	sh->envp[3] = ft_strdup(PATH_DEFAULT);
	safe_free_string(&cwd);
	if (!sh->envp[0] || !sh->envp[1] || !sh->envp[2] || !sh->envp[3])
		return (safe_free_2d_string(&sh->envp),
			ret_exit_perr(sh, ERRNO_CODE, E_INIT_ENV_MSG));
	return (sh->exit_code);
}
