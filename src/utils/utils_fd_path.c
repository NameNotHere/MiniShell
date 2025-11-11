/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:30:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_fd_path(int fd)
{
	char	*fd_str;
	char	*path;

	fd_str = ft_itoa(fd);
	if (!fd_str)
		return (NULL);
	path = ft_strjoin("/proc/self/fd/", fd_str);
	safe_free_str(&fd_str);
	return (path);
}
