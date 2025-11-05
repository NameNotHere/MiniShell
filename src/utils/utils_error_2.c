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
#include <string.h>
#include "minishell.h"

static void	print_chdir_error(char	*target_dir)
{
	if (errno == ENOTDIR)
		msg_err_2(E_CD_NOT_DIR, target_dir);
	else if (errno == EACCES)
		msg_err_2(E_CD_PERMISSION, target_dir);
	else if (errno == ENAMETOOLONG)
		msg_err_2(E_CD_NAME_TOO_LONG, target_dir);
	else
		msg_err_2(E_CD_NO_SUCH, target_dir);
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
	char		buf[4096];
	size_t		len;
	const char	*err_str;

	len = 0;
	ft_memcpy(buf, E_MINISHELL, sizeof(E_MINISHELL) - 1);
	len += sizeof(E_MINISHELL) - 1;
	while (*error && len < sizeof(buf) - 3)
		buf[len++] = *error++;
	if (len < sizeof(buf) - 2)
		buf[len++] = ':';
	if (len < sizeof(buf) - 2)
		buf[len++] = ' ';
	err_str = strerror(errno);
	while (*err_str && len < sizeof(buf) - 2)
		buf[len++] = *err_str++;
	if (len < sizeof(buf) - 1)
		buf[len++] = '\n';
	write(STDERR_FILENO, buf, len);
}
