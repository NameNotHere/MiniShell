/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:47:20 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:32:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "minishell.h"

static void	print_chdir_error(char	*target_dir)
{
	if (errno == ENOTDIR)
		msg_err_2("cd: not a directory: ", target_dir);
	else if (errno == EACCES)
		msg_err_2("cd: permission denied: ", target_dir);
	else if (errno == ENAMETOOLONG)
		msg_err_2("cd: file name too long: ", target_dir);
	else
		msg_err_2("cd: no such file or directory: ", target_dir);
}

int	set_dir_or_error(t_msh *sh, char **directory)
{
	char	*target_dir;
	int		res;

	res = EXIT_SUCCESS;
	if (ft_strcmp(*directory, "-") == 0)
		target_dir = get_env_value(sh, "OLDPWD",
				search_name("OLDPWD", sh->envp));
	else if (!*directory)
		target_dir = get_env_value(sh, "HOME", search_name("HOME", sh->envp));
	else if ((*directory)[0] == '\0')
		return (EXIT_SUCCESS);
	else
		target_dir = *directory;
	if (chdir(target_dir) != EXIT_SUCCESS)
	{
		print_chdir_error(target_dir);
		res = EXIT_FAILURE;
		errno = 0;
	}
	if (target_dir != *directory)
		free(target_dir);
	return (res);
}

void	ms_perror(const char *error)
{
	write(STDERR_FILENO, E_MINISHELL, sizeof(E_MINISHELL));
	perror(error);
}
