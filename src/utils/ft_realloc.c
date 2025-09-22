/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orhan    <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 18:24:55 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    *ft_realloc(void *ptr, size_t new_size, size_t old_size)
{
    void    *new_ptr;
    size_t  copy_size;

    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }
    if (!ptr)
        return (malloc(new_size));
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);
    if (old_size < new_size)
        copy_size = old_size;
    else
        copy_size = new_size;
    ft_memcpy(new_ptr, ptr, copy_size);

    free(ptr);
    return (new_ptr);
}
