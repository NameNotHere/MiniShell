/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:52:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 16:29:51 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/*
safe_free is the generic one works with any datatype or struct
	- cast to (void **) from whatever &datatype, it works
 */
void	safe_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_free_str(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_free_2d_string(char ***ptr)
{
	size_t	i;

	if (ptr && *ptr)
	{
		i = 0;
		while ((*ptr)[i])
			safe_free_str(&(*ptr)[i++]);
		free(*ptr);
		*ptr = NULL;
	}
}
