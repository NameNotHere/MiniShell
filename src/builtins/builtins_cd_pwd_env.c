/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd_pwd_env.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:44:50 by tda-roch         ###   ########.fr       */
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

