/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_and_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/21 03:07:28 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

int	ft_exit(t_msh *sh)
{
	free_everything(sh);
	exit_free_with_code(sh, EXIT_SUCCESS);
	sh->exit_code = errno;
	perror("failed exit");
	if (sh->exit_code == EXIT_SUCCESS)
		sh->exit_code = EXIT_FAILURE;
	return (sh->exit_code);
}