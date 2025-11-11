/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_malloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:15:47 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 13:58:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xe_malloc(void **ptr, int *err, size_t nmemb, size_t size)
{
	*ptr = NULL;
	*err = EXIT_SUCCESS;
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*err = ERANGE;
		return (*err);
	}
	*ptr = malloc(nmemb * size);
	if (!(*ptr))
	{
		*err = errno;
		if (!*err)
			*err = ENOMEM;
	}
	return (*err);
}

/*
|||xe_calloc: same as xe_malloc, but on successful allocation,
||zero initializes the allocated memory with ft_bzero.
*/
int	xe_calloc(void **ptr, int *err, size_t nmemb, size_t size)
{
	int	ret;

	ret = xe_malloc(ptr, err, nmemb, size);
	if (ret == EXIT_SUCCESS)
		ft_bzero(*ptr, nmemb * size);
	return (ret);
}
