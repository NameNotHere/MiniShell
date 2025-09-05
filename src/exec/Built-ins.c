/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/05 12:09:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	ft_echo(char *str, int with_arg_n)
{
	int	len;

	len = ft_strlen(str);
	if (with_arg_n == 1)
	{
		while (*str)
		{
			if (*str != '\n')
				write(1, str, 1);
			str++;
		}
		return (len);
	}
	return (write(1, str, len));
}

int	t_cd(char **envp, char *directory)
{
	struct stat	st;
	char		*pwd_value;
	char		*new_path;
	char		*temp;
	int			i;

	i = search_name("PWD", envp);
	if (i == -1)
		return (ft_echo("PWD not found\n", 0));
	pwd_value = envp[i] + length_till_equal(envp[i]) + 1;
	temp = ft_strjoin(pwd_value, "/");
	if (!temp)
		return (ft_echo("Memory error\n", 0));
	new_path = ft_strjoin(temp, directory);
	free(temp);
	if (!new_path)
		return (ft_echo("Memory error\n", 0));
	if (stat(new_path, &st) != 0 || !S_ISDIR(st.st_mode))
	{
		free(new_path);
		return (ft_echo("Invalid directory\n", 0));
	}
	temp = ft_strjoin("PWD=", new_path);
	free(new_path);
	if (!temp)
		return (ft_echo("Memory error\n", 0));
	free(envp[i]);
	envp[i] = temp;
	return (EXIT_SUCCESS);
}

int	ft_pwd(char **envp)
{
	int	i;
	int	equal;

	i = search_name("PWD", envp);
	if (i == -1)
		return (ft_echo("PWD not found\n", 0));
	equal = length_till_equal(envp[i]) + 1;
	ft_echo(envp[i] + equal, 0);
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int	execute_built_in(t_msh *sh, t_cmd *cmd)
{
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		ft_pwd(sh->envp);
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (t_cd(sh->envp, cmd->argv[1]));
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		ft_echo(cmd->argv[1], 0);
	else
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
