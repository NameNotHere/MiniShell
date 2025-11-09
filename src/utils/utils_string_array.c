/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string_array.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 23:22:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/09 12:22:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_string_array(char **strings)
{
	int		i;
	int		len;
	char	**copy;

	if (!strings || !strings[0])
		return (NULL);
	len = 0;
	while (strings[len])
		len++;
	copy = ft_calloc(len + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		copy[i] = ft_strdup(strings[i]);
		if (copy[i] == NULL)
		{
			safe_free_2d_string(&copy);
			return (NULL);
		}
		i++;
	}
	return (copy);
}

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
