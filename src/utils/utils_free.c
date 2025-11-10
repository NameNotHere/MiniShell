/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:52:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 17:01:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

/*
TODO: substitude most or all bare "free" operations of minishell with the
safe versions below.

Reasoning:
It is good to develop in debugging phase without the safe versions, so we get
useful errors that show problems with the code.
However, when the project is done, it is a good idea to swith to the safe
versions. Why?
- It makes sure all freed pointers are also set to NULL (no dangling pointers)
- It prevents errors with double-free in edge cases not tested during
	development phase.
- It is cool to use your own custom_wrapped & safe free.
*/

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
