
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

void	*ft_malloc(size_t amount, size_t size)
{
	void	*mal;

	mal = malloc(amount * size);
	if (!mal)
		printf("malloc failed\n");
	return (mal);
}


/*
malloc_check is a malloc wrapper with error return

Returns:
- 0 (EXIT_SUCCESS) if allocation worked.
- 12 (ENOMEM) if allocation failed.

NOTE: if allocation fails, also sets errno to ENOMEM

ENOMEM is defined in <errno.h> as default code for allocation error (12).
parameters:
- void **ptr = pointer to pointer to be allocated
	(must be passed by address (&ptr))
- size_t amount = number of items that could be allocated
- size_t size = sizeof(datatype)

usage example:

	char	*word;

	if (malloc_check((void **)&word, 11, sizeof(char)))
		return (EXIT_FAILURE);
*/
int	malloc_check(void **ptr, size_t nmemb, size_t size)
{
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*ptr = NULL;
		errno = ENOMEM;
		return (ENOMEM);
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
calloc_check: same as malloc_check, but on successful allocation,
	zero initializes the allocated memory with ft_bzero.
*/
int	calloc_check(void **ptr, size_t nmemb, size_t size)
{
	if (nmemb && size > SIZE_MAX / nmemb)
	{
		*ptr = NULL;
		errno = ENOMEM;
		return (ENOMEM);
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
