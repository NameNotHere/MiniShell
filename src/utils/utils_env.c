/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 18:28:25 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/30 02:38:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		set_exit_perr(sh, ERRNO_CODE, "get_env_value allocation failed");
		return (NULL);
	}
	return (var_value);
}
