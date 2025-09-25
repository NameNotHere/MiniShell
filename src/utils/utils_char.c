/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:03:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 18:54:27 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

/*
	check for valid variable characters
	Returns
		1 if valid
		0 is invalid
	Note:
		valid characters are: alphanumeric, underscore and question mark
*/
int	ft_valid_var_char(int c)
{
	if ('?' == c)
		return (1);
	if (ft_isalnum(c))
		return (1);
	if ('_' == c)
		return (1);
	return (0);
}

int	ft_is_singlequote(int c)
{
	if ('\'' == c)
		return (1);
	return (0);
}

int	ft_is_doublequote(int c)
{
	if ('\"' == c)
		return (1);
	return (0);
}

int	ft_is_quote(int c)
{
	if (ft_is_singlequote(c))
		return (1);
	if (ft_is_doublequote(c))
		return (1);
	return (0);
}
