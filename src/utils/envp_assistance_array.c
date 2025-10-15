/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_assistance_array.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:17:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 22:00:46 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdio.h>

int	length_till_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	search_name(char *name, char **envp)
{
	int	i;
	int	len_name;
	int	length_envp;

	if (!name)
		return (-1);
	i = 0;
	len_name = ft_strlen(name);
	while (envp[i])
	{
		length_envp = length_till_equal(envp[i]);
		if (ft_strncmp(name, envp[i], length_envp) == 0
			&& len_name == length_envp)
			return (i);
		i++;
	}
	return (-1);
}

int	change_env_value(char *name, char *new_value, char ***envp)
{
	int		index;
	char	*new_entry;

	index = search_name(name, *envp);
	if (index == -1)
		return (EXIT_FAILURE);
	new_entry = ft_strjoin3(name, "=", new_value);
	if (!new_entry)
		return (EXIT_FAILURE);
	free((*envp)[index]);
	(*envp)[index] = new_entry;
	return (EXIT_SUCCESS);
}

int	envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
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
