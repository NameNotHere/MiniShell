/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 20:50:46 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ft_exit(t_msh *sh, t_cmd cmd)
{
	if (cmd.argv[2])
	{
		put_stderr("exit: too many arguments\n");
		sh->exit_code = 1;
		return (sh->exit_code);
	}
	if (cmd.argv[1] && cmd.argv[1][0] != '\0' && ft_isdigit(cmd.argv[1][0]))
		sh->exit_code = ft_atoi(cmd.argv[1]);
	else if (cmd.argv[1])
		sh->exit_code = 0;
	exit_free_with_code(sh, sh->exit_code);
	return (sh->exit_code);
}

int	ft_export(t_msh **sh, t_cmd cmd)
{
	char	*name;
	char	*value;
	int		i;

	if (!cmd.argv[1] || !cmd.argv[3])
		return (EXIT_SUCCESS);
	name = cmd.argv[1];
	value = cmd.argv[3];
	i = search_name(name, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, name, value);
	else
		change_env_value(name, value, &(*sh)->envp);
	if (search_name(name, (*sh)->envp) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
