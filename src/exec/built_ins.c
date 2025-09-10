/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/05 16:44:15 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	ft_echo_one(t_cmd *cmd) // we didnt take care of the param
{
	int	i;
	int	len;

	len = ft_strlen(cmd->argv[1]);
	i = 0;
	if (cmd->argv[1][0] == '\"')
	{
		while (cmd->argv[0][i] && len > i++)
			if (cmd->argv[1][i] != '"')
				write(1, &(cmd->argv[1][i]), 1);
	}
	else
		return (write(1, cmd->argv[1], len));
	return (i);
}

int	ft_cd(char **envp, char *directory)
{
	struct stat	st;
	char		*pwd_value;
	char		*new_path;
	char		*temp;
	int			i;

	if (!directory)
		return (EXIT_FAILURE);
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
	if (ft_strncmp("..", directory, 2) != 0)
	{
		free(envp[i]);
		envp[i] = temp;
	}
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
	int ret;

	ret = EXIT_SUCCESS;
	if (!name)
		return (perror("no value defined to unset\n"), EXIT_FAILURE);
	ret = change_env_value(name, "\0", &(*sh)->envp);
	return (ret);
}

int	ft_export(t_msh **sh, t_cmd cmd)
{
	char	*name;
	char	*value;
	int		i;

	if (!cmd.argv[1])
		return (EXIT_SUCCESS);
	name = cmd.argv[1];
	value = cmd.argv[3];

	i = search_name(name, (*sh)->envp);
	if (i < 0)
		add_env_var(&(*sh)->envp, name, value);
	else
		change_env_value(name, value, &(*sh)->envp);
	if (search_name(name, (*sh)->envp) < 0)
		return (EXIT_FAILURE);

	return (EXIT_SUCCESS);
}

int	execute_built_in(t_msh *sh, t_cmd *cmd)
{
	execute_redirection(sh, cmd->redir);
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		ft_pwd(sh);
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (ft_cd(sh->envp, cmd->argv[1]));
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		return (ft_echo_one(cmd));
	else if (ft_strncmp(cmd->argv[0], "env", 3) == 0)
		return (ft_env(*sh));
	else if (ft_strncmp(cmd->argv[0], "export", 4) == 0)
		return (ft_export(&sh, *cmd));
	else if (ft_strncmp(cmd->argv[0], "unset", 4) == 0)
		return (ft_unset(&sh, cmd->argv[1]));
	else
		return (EXIT_FAILURE);
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
