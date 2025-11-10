/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 18:24:55 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** CUSTOM FUNCTION FOR MINISHELL
** Added to libft as part of custom minishell project build.
**
** NOTE: Similar to strndup() but with size_t replaced by int parameter.
**   - Duplicates up to 'size' characters from 'src'
**   - Stops at null terminator if src is shorter than size
**   - Returns newly allocated string, NULL on allocation failure
*/

#include <stdlib.h>
#include <string.h>

char	*ft_strndup(const char *src, int size)
{
	char	*ret;
	int		i;

	ret = malloc(sizeof(char) * (size + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (i < size && src[i])
	{
		ret[i] = src[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
