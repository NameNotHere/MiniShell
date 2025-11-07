/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 10:33:14 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/26 11:49:43 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** CUSTOM FUNCTION FOR MINISHELL
** Added to libft as part of custom minishell project build.
**
** NOTE: Differs from standard strcmp() - handles NULL pointers:
**   - Returns pointer difference (s1 - s2) if either pointer is NULL
**   - Returns standard character comparison result otherwise
**   This allows NULL pointer comparisons without segfault.
*/

#include <stdlib.h>

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (s1 - s2);
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
