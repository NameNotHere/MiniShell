/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_assistance_array.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:17:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 07:46:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	change_env_val_idx(char *name, char *new_value, int index, char ***envp)
{
	char	*new_entry;

	new_entry = ft_strjoin3(name, "=", new_value);
	if (!new_entry)
		return (EXIT_FAILURE);
	free((*envp)[index]);
	(*envp)[index] = new_entry;
	return (EXIT_SUCCESS);
}

/*
	Changes env value.
	Returns EXIT_SUCCESS if success.
	Returns EXIT_FAILURE if not.
	Note: fails silently (no message).
*/
int	change_env_val(char *name, char *new_value, char ***envp)
{
	int		index;

	if (!name || !new_value || name[0] == '\0')
		return (EXIT_FAILURE);
	index = search_name(name, *envp);
	if (index == -1)
		return (EXIT_FAILURE);
	return (change_env_val_idx(name, new_value, index, envp));
}

int	add_env_var(char ***envp, char *name, char *value)
{
	int		env_len;
	char	*new_entry;

	env_len = envp_len(*envp);
	if (!name || !value)
		return (EXIT_FAILURE);
	*envp = ft_realloc(*envp, sizeof(char *) * (env_len + 2),
			sizeof(char *) * env_len);
	if (!(*envp))
		return (EXIT_FAILURE);
	new_entry = ft_strjoin3(name, "=", value);
	if (!new_entry)
		return (EXIT_FAILURE);
	(*envp)[env_len] = new_entry;
	(*envp)[env_len + 1] = NULL;
	return (EXIT_SUCCESS);
}
