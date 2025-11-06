/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/06 14:47:06 by tda-roch         ###   ########.fr       */
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
		return (1);
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
	int	retval;

	retval = xe_malloc(ptr, err, nmemb, size);
	if (retval == EXIT_SUCCESS)
		ft_bzero(*ptr, nmemb * size);
	return (retval);
}

/*
|x_malloc: simple malloc wrapper without error pointer parameter
|Uses local error variable internally
*/
int	x_malloc(void **ptr, size_t nmemb, size_t size)
{
	int	local_err;

	return (xe_malloc(ptr, &local_err, nmemb, size));
}

/*
|x_calloc: simple calloc wrapper without error pointer parameter
|Uses local error variable internally
*/
int	x_calloc(void **ptr, size_t nmemb, size_t size)
{
	int	local_err;

	return (xe_calloc(ptr, &local_err, nmemb, size));
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

int	x_calloc_token(t_token **ptr, size_t count)
{
	int	local_err;

	return (xe_calloc_token(ptr, &local_err, count));
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
