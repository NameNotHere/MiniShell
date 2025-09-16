/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_assis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:17:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/13 19:32:23 by tda-roch         ###   ########.fr       */
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
	char	*str;

	index = search_name(name, *envp);
	if (index == -1)
		return (EXIT_FAILURE);
	str = ft_strjoin(name, "=");
	new_entry = ft_strjoin(str, new_value);
	free(str);
	if (!new_entry)
		return (EXIT_FAILURE);
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

	env_len = envp_len(*envp);
	*envp = realloc(*envp, sizeof(char *) * (env_len + 2));
	if (!(*envp))
		return (EXIT_FAILURE);
	(*envp)[env_len] = name;
	(*envp)[env_len + 1] = NULL;
	change_env_value(name, value, envp);
	return (EXIT_SUCCESS);
}
