/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 12:46:28 by otanovic          #+#    #+#             */
/*   Updated: 2025/09/23 16:48:04 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	is_closed(char *str, int i, char quote)
{
	while (str[i])
	{
		if (str[i] == quote)
			return (0);
		i++;
	}
	return (1);
}

int	unclosed_token(char *str, char token)
{
	if (token == 0)
	{
		if (is_closed(str, 0, '\'') == 1)
			return (2);
		if (is_closed(str, 0, '\"') == 1)
			return (3);
	}
	else if (token && is_closed(str, 0, token) == 1)
		return (1);
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
int	callo_x(void **ptr, size_t nmemb, size_t size)
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

// a nice error-printing malloc (may be useful somewhere)
// void	*ft_malloc(size_t amount, size_t size)
// {
// 	void	*mal;

// 	mal = malloc(amount * size);
// 	if (!mal)
// 		printf("malloc failed\n");
// 	return (mal);
// }

/*
	validate_pipe_syntax: checks if pipe has valid commands on both sides
	Returns:
		- 0 if syntax is valid
		- 2 if syntax error
*/
int	validate_pipe_syntax(t_token *tokens, int start, int end)
{
	int		i;
	bool	found_pipe;
	bool	has_left_cmd;
	bool	has_right_cmd;

	i = start;
	found_pipe = false;
	has_left_cmd = false;
	has_right_cmd = false;
	while (tokens[i].word && i <= end && tokens[i].ty != TOKEN_PIPE)
	{
		if (tokens[i].ty == TOKEN_WORD)
			has_left_cmd = true;
		i++;
	}
	if (tokens[i].ty == TOKEN_PIPE)
	{
		found_pipe = true;
		i++;
		while (tokens[i].word && i <= end)
		{
			if (tokens[i].ty == TOKEN_WORD)
				has_right_cmd = true;
			i++;
		}
	}
	if (found_pipe && (!has_left_cmd || !has_right_cmd))
		return (2);
	return (0);
}
