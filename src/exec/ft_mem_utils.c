/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:48:28 by tda-roch          #+#    #+#             */
/*   Updated: 2025/04/15 14:22:40 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

/*
Copies  n bytes from memory area src to memory
area dest.  The memory areas must not overlap.  Use memmove if the
memory areas do overlap.
RETURN VALUE
	The memcpy() function returns a pointer to dest.
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*uc_src;
	unsigned char	*uc_dst;

	if (dst == NULL || src == NULL)
		return (NULL);
	if (src == dst)
		return (dst);
	uc_src = (unsigned char *)src;
	uc_dst = (unsigned char *)dst;
	i = 0;
	while (i < n)
	{
		uc_dst[i] = uc_src[i];
		i++;
	}
	return (dst);
}

/*
fills the first n bytes of the memory area pointed to by s
with the constant byte c.
*/

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*new_s;
	unsigned char	uc;

	uc = (unsigned char)c;
	new_s = (unsigned char *)s;
	while (n--)
		*new_s++ = uc;
	return (s);
}

/*
erases the data in the n bytes of the memory starting at the
location pointed to by s, by writing zeros (bytes containing '\0') to that area.
RETURN VALUE: None.
*/
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

/*
allocates memory for an array of nmemb elements of size bytes each and returns a
pointer to the allocated memory.  The memory is set to zero.
If nmemb or size is 0, then calloc returns a unique pointer value that can
later be successfully passed to free().

If the multiplication of nmemb and size would result in integer overflow, then
calloc returns an error.
By contrast, an integer overflow would not be detected in the following call to
malloc, with the result that an incorrectly sized block of memory would be
allocated.
*/
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	p = malloc(nmemb * size);
	if (!p)
		return (NULL);
	ft_bzero(p, (nmemb * size));
	return (p);
}
