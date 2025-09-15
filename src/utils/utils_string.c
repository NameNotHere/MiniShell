/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:42:11 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/13 23:18:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns an empty string
	TODO: check if we need to catch error here or on the caller.
	Probably the latter
*/
char	*get_empty_string(void)
{
	return (ft_calloc(1, sizeof(char)));
}

/*
	Sets a string to an empty string.
	Returns true on success, false on failure.

*/
bool	set_empty_string(char **to_empty)
{
	safe_free_string(to_empty);
	*to_empty = get_empty_string();
	if (*to_empty == NULL)
		return (false);
	return (true);
}
