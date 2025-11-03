/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/03 12:09:17 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>


/*
	If getcwd fails (for example when current directory was removed),
	prefer to keep the existing PWD rather than overwriting it with a
	possibly-invalid path. Only use oldpwd as a fallback when it still
	points to a valid directory.
	leave cwd NULL to indicate unknown cwd; do not update PWD
*/
int	ft_cd(t_msh *sh, t_cmd *cmd)
{
	struct stat	sb;
	int			pwd_index;
	char		*cwd;
	char		*oldpwd;

	if (cmd->argc > 2)
		return (msg_err("cd: too many arguments"), EXIT_FAILURE);
	oldpwd = get_env_value(sh, "PWD", search_name("PWD", sh->envp));
	if (set_dir_or_error(sh, &cmd->argv[1]) == EXIT_FAILURE)
		return (ret_free_string(&oldpwd, EXIT_FAILURE));
	cwd = getcwd(NULL, 0);

	pwd_index = search_name("PWD", sh->envp);
	if (cwd == NULL)
	{
		if (oldpwd && stat(oldpwd, &sb) == 0 && S_ISDIR(sb.st_mode))
			cwd = ft_strdup(oldpwd);
		else
			cwd = NULL;
	}
	if (cwd)
	{
		if (pwd_index != -1)
		{
			free(sh->envp[pwd_index]);
			sh->envp[pwd_index] = ft_strjoin("PWD=", cwd);
		}
		else
			add_env_var(&sh->envp, "PWD", cwd);
		safe_free_string(&cwd);
	}
	if (!sh->envp[pwd_index])
		return (ret_msg_free_string(E_CD_ALLOC, &oldpwd, EXIT_FAILURE));
	change_env_value("OLDPWD", oldpwd, &sh->envp);
	safe_free_string(&oldpwd);
	return (EXIT_SUCCESS);
}

int	ft_pwd(t_msh *sh, t_cmd *cmd)
{
	int	i;
	int	equal;

	(void)cmd;
	i = search_name("PWD", sh->envp);
	if (i == -1)
	{
		msg_err("PWD not found");
		return (EXIT_FAILURE);
	}
	equal = length_till_equal(sh->envp[i]) + 1;
	write(1, sh->envp[i] + equal, ft_strlen(sh->envp[i] + equal));
	write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int	ft_env(t_msh *sh, int argc)
{
	int	i;

	if (argc > 1)
		return (ret_exit_msg(sh, 127, "env: arguments not supported"));
	i = 0;
	while (sh->envp[i])
	{
		write(1, sh->envp[i], ft_strlen(sh->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_unset(t_msh **sh, char *name)
{
	int	i;
	int	env_len;

	if (!name)
		return (EXIT_SUCCESS);
	if (name[0] == '-')
	{
		msg_err_3("unset: ", name, ": invalid option");
		return (2);
	}
	i = search_name(name, (*sh)->envp);
	if (i == -1)
		return (EXIT_SUCCESS);
	free((*sh)->envp[i]);
	env_len = envp_len((*sh)->envp);
	while (i < env_len - 1)
	{
		(*sh)->envp[i] = (*sh)->envp[i + 1];
		i++;
	}
	(*sh)->envp[env_len - 1] = NULL;
	if (ft_strcmp(name, "PATH") == 0)
		update_path_dirs(&(*sh)->path_dirs, (*sh)->envp);
	return (EXIT_SUCCESS);
}

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	int	ret;

	ret = 127;
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ret = ft_pwd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ret = ft_cd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ret = ft_echo(cmd->argv, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ret = ft_env(sh, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ret = ft_export(&sh, *cmd);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ret = ft_unset(&sh, cmd->argv[1]);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ret = ft_exit(sh, *cmd);
	return (ret);
}
