/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/11 18:21:42 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

char	*write_var(char **envp, char *name)
{
	int	len_name;
	int	i;

	if (!name)
		return (NULL);
	i = 0;
	len_name = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len_name) == 0 && envp[i][len_name] == '=')
			return (envp[i] + len_name);
		i++;
	}
	return (NULL);
}

char	*till_space(char *str)
{
	int		i;
	char	*ret;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != ' ')
		i++;
	ret = malloc(i + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, str, i + 1);
	return (ret);
}

int	ft_nflags(char **args)
{
	int	z;
	int	i;

	z = 1;
	while (args[z])
	{
		if (args[z][0] != '-')
			break ;
		i = 1;
		while (args[z][i] == 'n')
			i++;
		if (args[z][i] != '\0')
			break ;
		z++;
	}
	return (z > 1);
}


int	ft_echo(t_cmd *cmd, t_msh *sh)
{
	int		i;
	int		start;
	char	*var_value;
	char	*var_name;
	int		z;

	z = 1;
	i = 0;
	while (cmd->argv[z])
	{
		if (ft_strncmp(cmd->argv[z], "-n", 2) == 0)
		{
			z++;
			continue ;
		}
		while (cmd->argv[z][i])
		{
			if (cmd->argv[z][i] == '"')
			{
				i++;
				continue ;
			}
			else if (cmd->argv[z][i] == '$')
			{
				start = ++i;
				while (cmd->argv[z][i] && cmd->argv[z][i] != ' ')
					i++;
				var_name = strndup(&cmd->argv[z][start], i - start);
				if (!var_name)
					return (1);
				var_value = write_var(sh->envp, var_name);
				if (var_value)
					write(1, var_value, ft_strlen(var_value));
				free(var_name);
			}
			else
			{
				write(1, &cmd->argv[z][i], 1);
				i++;
			}
		}
		i = 0;
		z++;
		if (cmd->argv[z])
			write(1, " ", 1);
	}
	if (ft_nflags(cmd->argv) == 0)
		write(1, "\n", 1);
	return (0);
}

char	*remove_last_folder(char *path)
{
	int		i;
	int		last_slash;
	char	*ret;

	i = 0;
	last_slash = -1;
	while (path[i])
	{
		if (path[i] == '/')
			last_slash = i;
		i++;
	}
	if (last_slash <= 0)
		return (ft_strdup("/"));
	ret = malloc(last_slash + 1);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, path, last_slash + 1);

	return (ret);
}

int	ft_cd(char ***envp, char *directory)
{
	int		pwd;
	char	*new_dir;
	char	*free_dir;

	pwd = search_name("PWD", *envp);
	if (ft_strncmp(directory, "..", 3) == 0)
	{
		new_dir = (*envp)[pwd];
		(*envp)[pwd] = remove_last_folder((*envp)[pwd]);
		free(new_dir);
		return (EXIT_SUCCESS);
	}
	free_dir = (*envp)[pwd];
	if (directory[0] == '/' && directory[1] == '\0')
		(*envp)[pwd] = ft_strjoin("PWD=", "/"); //doesnt work
	if (directory[0] == '~' && directory[1] == 0)
		(*envp)[pwd] = ft_strdup((*envp)[search_name("HOME", *envp)]);
	if ((directory[0] == '/' || directory[0] == '~') && directory[1] == 0)
	{
		free(free_dir);
		return (EXIT_SUCCESS);
	}
	new_dir = ft_strjoin3((ft_strchr((*envp)[pwd], '=') + 1), "/", directory);
	if (chdir(new_dir) == 0)
	{
		free_dir = (*envp)[pwd];
		(*envp)[pwd] = ft_strjoin("PWD=", new_dir);
		free(free_dir);
		free(new_dir);
		return (EXIT_SUCCESS);
	}
	free(new_dir);
	perror("Invalid Directory\n");
	return (EXIT_FAILURE);
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

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	execute_redirection(sh, cmd->redir);
	if (ft_strncmp(cmd->argv[0], "pwd", 3) == 0)
		ft_pwd(sh);
	else if (ft_strncmp(cmd->argv[0], "cd", 2) == 0)
		return (ft_cd(&sh->envp, cmd->argv[1]));
	else if (ft_strncmp(cmd->argv[0], "echo", 4) == 0)
		return (ft_echo(cmd, sh));
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
