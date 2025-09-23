/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 20:36:45 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	ft_cd(char ***envp, char *directory)
{
	int		pwd_index;
	char	*cwd;

	if (!directory)
		return (EXIT_FAILURE);
	if (chdir(directory) != 0)
	{
		perror("cd");
		return (EXIT_FAILURE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (EXIT_FAILURE);
	}
	pwd_index = search_name("PWD", *envp);
	if (pwd_index >= 0)
	{
		free((*envp)[pwd_index]);
		(*envp)[pwd_index] = ft_strjoin("PWD=", cwd);
	}
	free(cwd);
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_msh *sh)
{
	int	i;
	int	equal;

	i = search_name("PWD", sh->envp);
	if (i == -1)
	{
		write(STDERR_FILENO, "PWD not found\n", 14);
		return (EXIT_FAILURE);
	}
	equal = length_till_equal(sh->envp[i]) + 1;
	write(1, sh->envp[i] + equal, ft_strlen(sh->envp[i] + equal));
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int	ft_env(t_msh sh)
{
	int	i;

	i = 0;
	while (sh.envp[i])
	{
		write(1, sh.envp[i], ft_strlen(sh.envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_unset(t_msh **sh, char *name)
{
	int	ret;

	ret = EXIT_SUCCESS;
	if (!name)
		return (perror("no value defined to unset\n"), EXIT_FAILURE);
	ret = change_env_value(name, "\0", &(*sh)->envp);
	return (ret);
}

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		return (ft_pwd(sh));
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (ft_cd(&sh->envp, cmd->argv[1]));
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		return (ft_echo(cmd->argv));
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
		return (ft_env(*sh));
	else if (ft_strncmp(cmd->argv[0], "export", 4) == 0)
		return (ft_export(&sh, *cmd));
	else if (ft_strncmp(cmd->argv[0], "unset", 4) == 0)
		return (ft_unset(&sh, cmd->argv[1]));
	else if (ft_strncmp(cmd->argv[0], "exit", 4) == 0)
		return (ft_exit(sh));
	put_stderr("error: unknown built_in");
	return (EXIT_FAILURE);
}
