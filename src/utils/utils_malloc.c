/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_malloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/11 11:27:45 by tda-roch         ###   ########.fr       */
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
||xe_calloc: same as xe_malloc, but on successful allocation,
|zero initializes the allocated memory with ft_bzero.
*/
int	xe_calloc(void **ptr, int *err, size_t nmemb, size_t size)
{
	int	ret;

	ret = xe_malloc(ptr, err, nmemb, size);
	if (ret == EXIT_SUCCESS)
		ft_bzero(*ptr, nmemb * size);
	return (ret);
}

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
