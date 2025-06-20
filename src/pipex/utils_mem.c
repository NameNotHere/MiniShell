/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:11:54 by tda-roch          #+#    #+#             */
/*   Updated: 2025/04/15 14:23:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

void	*realloc_with_oldsize(void *ptr, size_t old_size, size_t new_size)
{
	void	*realloced_ptr;
	size_t	copy_len;

	if (!ptr)
		return (ft_calloc(new_size, 1));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	realloced_ptr = ft_calloc(new_size, 1);
	if (!realloced_ptr)
		return (NULL);
	if (old_size < new_size)
		copy_len = old_size;
	else
		copy_len = new_size;
	ft_memcpy(realloced_ptr, ptr, copy_len);
	free(ptr);
	return (realloced_ptr);
}
