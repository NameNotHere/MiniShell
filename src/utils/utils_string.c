/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 09:42:11 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/05 09:48:33 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*Returns an empty string
TODO: use our custom callo_x instead to catch error
*/
char	*get_empty_string(void)
{
	char	*empty_string;

	empty_string = ft_calloc(1, sizeof(char));
	if (empty_string == NULL)
		return (NULL);
	return (empty_string);
}
