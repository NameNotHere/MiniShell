/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:52:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/04/11 12:47:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"

void	safe_free_bool(bool **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_free_2d(char ***ptr)
{
	size_t	i;

	if (ptr && *ptr)
	{
		i = 0;
		while ((*ptr)[i])
			safe_free(&(*ptr)[i++]);
		free(*ptr);
		*ptr = NULL;
	}
}

void	safe_free_3d(char ****ptr)
{
	size_t	i;

	if (ptr && *ptr)
	{
		i = 0;
		while ((*ptr)[i])
		{
			safe_free_2d(&(*ptr)[i]);
			i++;
		}
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_everything(t_pipex *px)
{
	safe_free_2d(&px->path_dirs);
	safe_free_2d(&px->cmd_path);
	safe_free_3d(&px->cmd_arg);
	safe_free_bool(&px->cmd_not_found);
	if (px->fdin != STDIN_FILENO)
		close(px->fdin);
	if (px->fdout != STDOUT_FILENO)
		close(px->fdout);
}
