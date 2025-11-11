/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_malloc_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 10:15:47 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 13:58:58 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Type-specific wrappers for calloc without error pointer */

int	x_calloc_char(char **ptr, size_t count)
{
	int	local_err;

	return (xe_calloc_char(ptr, &local_err, count));
}

int	x_calloc_int(int **ptr, size_t count)
{
	int	local_err;

	return (xe_calloc_int(ptr, &local_err, count));
}

int	x_calloc_charptr(char ***ptr, size_t count)
{
	int	local_err;

	return (xe_calloc_charptr(ptr, &local_err, count));
}

int	x_calloc_redir(t_redir **ptr, size_t count)
{
	int	local_err;

	return (xe_calloc((void **)ptr, &local_err, count, sizeof(t_redir)));
}

int	x_calloc_ast(t_ast **ptr, size_t count)
{
	int	local_err;

	return (xe_calloc((void **)ptr, &local_err, count, sizeof(t_ast)));
}
