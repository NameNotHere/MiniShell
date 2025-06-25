/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:46:52 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/25 12:56:59 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	is_closed(char *str, int i, char quote)
{
	while (str[i])
	{
		if (str[i] == quote)
			return ;
		i++;
	}
	printf("unclosed quotes\n");
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

void	free_tokens(t_token *tokens, int amount)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (amount--)
	{
		free(tokens[i].word);
		i++;
	}
	if (tokens)
		free(tokens);
}
