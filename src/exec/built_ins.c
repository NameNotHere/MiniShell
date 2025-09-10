/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/10 17:46:03 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	ft_echo(char *str, int with_arg_n)
{
	int		len;

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
	{
		write(STDERR_FILENO, "PWD not found\n", 14);
		return (EXIT_FAILURE);
	}
	pwd_value = envp[i] + length_till_equal(envp[i]) + 1;
	temp = ft_strjoin(pwd_value, "/");
	if (!temp)
		return (perror("cd ft_strjoin 1"), EXIT_FAILURE);
	new_path = ft_strjoin(temp, directory);
	free(temp);
	if (!new_path)
		return (perror("cd ft_strjoin 2"), EXIT_FAILURE);
	if (stat(new_path, &st) != 0 || !S_ISDIR(st.st_mode))
	{
		free(new_path);
		write(STDERR_FILENO, "Invalid directory\n", 18);
		return (EXIT_SUCCESS);
	}
	if (chdir(new_path) != EXIT_SUCCESS)
	{
		free(new_path);
		perror("chdir");
		return (EXIT_FAILURE);
	}
	temp = ft_strjoin("PWD=", new_path);
	free(new_path);
	if (!temp)
		return (perror("cd ft_strjoin 3"), EXIT_FAILURE);
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
	{
		write(STDERR_FILENO, "PWD not found\n", 14);
		return (EXIT_FAILURE);
	}
	equal = length_till_equal(envp[i]) + 1;
	ft_echo(envp[i] + equal, 0);
	return (EXIT_SUCCESS);
}

void	ft_env(t_msh sh)
{
	int len;
	int	i;

	i = 0;
	while (sh.envp[i] != NULL)
	{
		len = ft_strlen(sh.envp[i]);
		write(1, sh.envp[i], len);
		write(1, "\n", 1);
		i++;
	}
}

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	int	i;

	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		sh->exit_code = ft_pwd(sh->envp);
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		sh->exit_code = t_cd(sh->envp, cmd->argv[1]);
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
	{
		i = 1;
		while (cmd->argv[i])
		{
			ft_echo(cmd->argv[i], 0);
			if (cmd->argv[i + 1])
				write(1, " ", 1);
			i++;
		}
		sh->exit_code = EXIT_SUCCESS;
	}
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
	{
		ft_env(*sh);
		sh->exit_code = EXIT_SUCCESS;
	}
	else
		sh->exit_code = EXIT_FAILURE;
	if (sh->exit_code == EXIT_SUCCESS)
		write(1, "\n", 1);
	return (sh->exit_code);
}
