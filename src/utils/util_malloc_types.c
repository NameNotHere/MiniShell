/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_malloc_types.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:15:47 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 13:58:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Type-specific wrappers for calloc only */

int	xe_calloc_char(char **ptr, int *err, size_t count)
{
	return (xe_calloc((void **)ptr, err, count, sizeof(char)));
}

int	xe_calloc_token(t_token **ptr, int *err, size_t count)
{
	return (xe_calloc((void **)ptr, err, count, sizeof(t_token)));
}

int	xe_calloc_int(int **ptr, int *err, size_t count)
{
	return (xe_calloc((void **)ptr, err, count, sizeof(int)));
}

int	xe_calloc_charptr(char ***ptr, int *err, size_t count)
{
	return (xe_calloc((void **)ptr, err, count, sizeof(char *)));
}
