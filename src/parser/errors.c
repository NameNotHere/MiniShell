/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/05 01:28:18 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell_parser.h"

bool	is_quote_closed(const char *str, char quote)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == quote && (i == 0 || str[i - 1] != '\\'))
			return (true);
		i++;
	}
	return (false);
}

int	unclosed_token(const char *str, char token)
{
	if (token == 0)
	{
		if (!is_quote_closed(str, '\''))
			return (2);
		if (!is_quote_closed(str, '\"'))
			return (3);
	}
	else if (!is_quote_closed(str, token))
	{
		return (1);
	}
	return (0);
}

int	is_closed(char *str, int i, char quote)
{
	while (str[i])
	{
		if (str[i] == quote)
			return (1);
		i++;
	}
	return (0);
}

/*
x_malloc is a malloc wrapper with error return

Returns:
- 0 (EXIT_SUCCESS) if allocation worked.
- 12 (ENOMEM) if allocation failed.
- 34 (ERANGE) or "value out of range" (nmemb * size would exceed SIZE_MAX)

NOTE: if allocation fails, also sets errno to ENOMEM

ENOMEM is defined in <errno.h> as default code for allocation error (12).
parameters:
- void **ptr = pointer to pointer to be allocated
	(must be passed by address (&ptr))
- size_t amount = number of items that could be allocated
- size_t size = sizeof(datatype)

usage example:

	char	*word;

	if (x_malloc((void **)&word, 11, sizeof(char)))
		return (EXIT_FAILURE);
*/
int	x_malloc(void **ptr, int *err, size_t nmemb, size_t size)
{
	int	local_err;
	int	*err_ptr;

	if (!err)
		err_ptr = &local_err;
	else
		err_ptr = err;
	*err_ptr = EXIT_SUCCESS;
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*ptr = NULL;
		errno = ERANGE;
		*err_ptr = ERANGE;
		return (*err_ptr);
	}
	*ptr = malloc(nmemb * size);
	if (!(*ptr))
	{
		errno = ENOMEM;
		*err_ptr = ENOMEM;
	}
	return (*err_ptr);
}

/*
callo_x: same as x_malloc, but on successful allocation,
zero initializes the allocated memory with ft_bzero.
*/
int	x_calloc(void **ptr, int *err, size_t nmemb, size_t size)
{
	int	retval;

	retval = x_malloc(ptr, err, nmemb, size);
	if (retval == EXIT_SUCCESS)
		ft_bzero(*ptr, nmemb * size);
	return (retval);
}

/* Type-specific wrappers for common types */

int	x_malloc_char(char **ptr, int *err, size_t count)
{
	return (x_malloc((void **)ptr, err, count, sizeof(char)));
}

int	x_calloc_char(char **ptr, int *err, size_t count)
{
	return (x_calloc((void **)ptr, err, count, sizeof(char)));
}

int	x_malloc_token(t_token **ptr, int *err, size_t count)
{
	return (x_malloc((void **)ptr, err, count, sizeof(t_token)));
}

int	x_calloc_token(t_token **ptr, int *err, size_t count)
{
	return (x_calloc((void **)ptr, err, count, sizeof(t_token)));
}

int	x_calloc_int(int **ptr, int *err, size_t count)
{
	return (x_calloc((void **)ptr, err, count, sizeof(int)));
}
