/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 20:43:38 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	x_cd(t_msh *sh, t_cmd *cmd)
{
	int			pwd_idx;
	char		*cwd;
	char		*oldpwd;

	if (cmd->argc > 2)
		return (r_msg_err(E_CD_TOO_MANY, EXIT_FAILURE));
	oldpwd = get_env_value_by_name(sh, "PWD");
	if (change_dir_or_error(sh, &cmd->argv[1]) == EXIT_FAILURE)
		return (r_free_str(&oldpwd, EXIT_FAILURE));
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (r_msg_err_free_str(E_CD_CWD_NULL, &oldpwd, EXIT_SUCCESS));
	pwd_idx = search_name("PWD", sh->envp);
	if ((pwd_idx == -1 && add_env_var(&sh->envp, "PWD", cwd) != EXIT_SUCCESS)
		|| (pwd_idx != -1 && change_env_val_idx("PWD", cwd, pwd_idx, &sh->envp)
			!= EXIT_SUCCESS))
		return (r_free_two_str(&cwd, &oldpwd, EXIT_FAILURE));
	safe_free_str(&cwd);
	pwd_idx = search_name("PWD", sh->envp);
	if (pwd_idx == -1 || !sh->envp[pwd_idx])
		return (r_msg_err_free_str(E_ALLOC_CD, &oldpwd, EXIT_FAILURE));
	change_env_val("OLDPWD", oldpwd, &sh->envp);
	safe_free_str(&oldpwd);
	return (EXIT_SUCCESS);
}

int	x_pwd(t_msh *sh, t_cmd *cmd)
{
	int	i;
	int	equal;

	(void)cmd;
	i = search_name("PWD", sh->envp);
	if (i == -1)
	{
		msg_err(E_PWD_NOT_FOUND);
		return (EXIT_FAILURE);
	}
	equal = length_till_equal(sh->envp[i]) + 1;
	(void)write(1, sh->envp[i] + equal, ft_strlen(sh->envp[i] + equal));
	(void)write(1, "\n", 1);
	return (EXIT_SUCCESS);
}

int	x_env(t_msh *sh, int argc)
{
	int	i;

	if (argc > 1)
		return (r_set_exit_msg(sh, EXIT_CMD_NOT_FOUND, E_ENV_ARG_NOT_SUPPORT));
	i = 0;
	while (sh->envp[i])
	{
		(void)write(1, sh->envp[i], ft_strlen(sh->envp[i]));
		(void)write(1, "\n", 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	unset_single_var(t_msh *sh, char *name)
{
	int	i;
	int	env_len;

	if (name[0] == '-')
	{
		msg_err_3(E_UNSET_START, name, E_UNSET_END);
		return (EXIT_SYNTAX);
	}
	i = search_name(name, sh->envp);
	if (i == -1)
		return (EXIT_SUCCESS);
	free(sh->envp[i]);
	env_len = envp_len(sh->envp);
	while (i < env_len - 1)
	{
		sh->envp[i] = sh->envp[i + 1];
		i++;
	}
	sh->envp[env_len - 1] = NULL;
	if (ft_strcmp(name, "PATH") == 0)
		update_path_dirs(&sh->path_dirs, sh->envp);
	return (EXIT_SUCCESS);
}

int	x_unset(t_msh *sh, t_cmd cmd)
{
	int	arg_idx;
	int	exit_code;
	int	ret;

	if (!cmd.argv[1])
		return (EXIT_SUCCESS);
	arg_idx = 1;
	exit_code = EXIT_SUCCESS;
	while (arg_idx < cmd.argc)
	{
		ret = unset_single_var(sh, cmd.argv[arg_idx]);
		if (ret != EXIT_SUCCESS)
			exit_code = ret;
		arg_idx++;
	}
	return (exit_code);
}

int	execute_builtin(t_msh *sh, t_cmd *cmd)
{
	int	ret;

	ret = EXIT_CMD_NOT_FOUND;
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ret = x_pwd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ret = x_cd(sh, cmd);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ret = x_echo(cmd->argv, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ret = x_env(sh, cmd->argc);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ret = x_export(sh, *cmd);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ret = x_unset(sh, *cmd);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ret = x_exit(sh, *cmd);
	return (ret);
}
