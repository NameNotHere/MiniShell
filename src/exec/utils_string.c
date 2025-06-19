/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 01:17:38 by tda-roch          #+#    #+#             */
/*   Updated: 2025/04/14 21:14:08 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/*Returns an empty string

Return value:
	Empty (char*) string (null terminated)
	If allocation fails, returns NULL
*/
char	*get_empty_string(void)
{
	char	*empty_string;

	empty_string = malloc(sizeof(char));
	if (empty_string == NULL)
		return (NULL);
	empty_string[0] = '\0';
	return (empty_string);
}

/*
Returns true if char found in a string.

Parameters:
	c_to_find: The char to be found
	str: The string to be searched
Return value:
	Returns 1 if found.
	if not found, returns (0)
*/
int	c_in_str(char c_to_find, char *str)
{
	size_t	c_index;

	c_index = 0;
	while (str[c_index] != '\0')
	{
		if (str[c_index] == c_to_find)
			return (1);
		c_index++;
	}
	return (0);
}
