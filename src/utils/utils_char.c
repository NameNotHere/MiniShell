/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 10:03:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/27 13:43:10 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

bool	ft_valid_var_char(int c)
{
	if (ft_isalnum(c))
		return (true);
	if ('_' == c)
		return (true);
	return (false);
}

bool	is_sgl_quote(int c)
{
	if ('\'' == c)
		return (true);
	return (false);
}

bool	is_dbl_quote(int c)
{
	if ('\"' == c)
		return (true);
	return (false);
}
