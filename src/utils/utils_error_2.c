/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 13:47:20 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 19:48:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	get_home_value(t_msh *sh, char **target_dir)
{
	*target_dir = get_env_value_by_name(sh, "HOME");
	if (!*target_dir || *target_dir[0] == '\0')
	{
		safe_free_str(target_dir);
		*target_dir = ft_strdup(getenv("HOME"));
	}
	if (!*target_dir)
		*target_dir = ft_strdup("");
}

int	change_dir_or_error(t_msh *sh, char **directory)
{
	char	*target_dir;
	int		ret;

	target_dir = NULL;
	ret = EXIT_SUCCESS;
	if (ft_strcmp(*directory, "-") == 0)
		target_dir = get_env_value_by_name(sh, "OLDPWD");
	else if (!*directory)
		get_home_value(sh, &target_dir);
	else if ((*directory)[0] == '\0')
		return (EXIT_SUCCESS);
	else
		target_dir = *directory;
	if (chdir(target_dir) != EXIT_SUCCESS)
	{
		print_chdir_error(target_dir);
		ret = EXIT_FAILURE;
		errno = 0;
	}
	if (target_dir != *directory)
		free(target_dir);
	return (ret);
}

/*
	Prints minishell error message with errno information.
*/
void	msg_perr(const char *error)
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
	(void)write(STDERR_FILENO, buf, len);
}
