/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/30 23:39:20 by tda-roch         ###   ########.fr       */
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
mallo_x is a malloc wrapper with error return

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

	if (mallo_x((void **)&word, 11, sizeof(char)))
		return (EXIT_FAILURE);
*/
int	mallo_x(void **ptr, size_t nmemb, size_t size)
{
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*ptr = NULL;
		errno = ERANGE;
		return (ERANGE);
	}
	*ptr = malloc(nmemb * size);
	if (!(*ptr))
	{
		errno = ENOMEM;
		return (ENOMEM);
	}
	return (EXIT_SUCCESS);
}

/*
callo_x: same as mallo_x, but on successful allocation,
	zero initializes the allocated memory with ft_bzero.
*/
int	x_calloc(void **ptr, size_t nmemb, size_t size)
{
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*ptr = NULL;
		errno = ERANGE;
		return (ERANGE);
	}
	*ptr = malloc(nmemb * size);
	if (!(*ptr))
	{
		errno = ENOMEM;
		return (ENOMEM);
	}
	ft_bzero(*ptr, nmemb * size);
	return (EXIT_SUCCESS);
}
