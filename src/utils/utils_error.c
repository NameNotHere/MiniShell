/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:04:21 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/21 02:24:40 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "minishell.h"

void	msg_err(const char *error)
{
	while (*error)
		write(STDERR_FILENO, error++, 1);
}

void	msg_err_2(const char *str1, const char *str2)
{
	msg_err(str1);
	msg_err(str2);
}

void	msg_err_3(const char *str1, const char *str2, const char *str3)
{
	msg_err(str1);
	msg_err(str2);
	msg_err(str3);
}

int	msg_err_and_free_string(const char *str1, char **to_free)
{
	msg_err(str1);
	safe_free_string(to_free);
	return (EXIT_FAILURE);
}

int	set_dir_or_error(t_msh *sh, char **directory)
{
	if (ft_strcmp(*directory, "-") == 0)
	{
		safe_free_string(directory);
		*directory = get_env_value(sh, "OLDPWD",
				search_name("OLDPWD", sh->envp));
	}
	else if (!*directory || ft_strcmp(*directory, "~") == 0)
	{
		safe_free_string(directory);
		*directory = get_env_value(sh, "HOME", search_name("HOME", sh->envp));
	}
	if (chdir(*directory) != EXIT_SUCCESS)
	{
		msg_err_3("cd: no such file or directory: ", *directory, "\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
