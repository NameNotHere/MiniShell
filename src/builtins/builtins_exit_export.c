/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/30 11:33:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

bool	is_valid_exit_code(const char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	ft_exit(t_msh *sh, t_cmd cmd)
{
	int			exit_code;

	exit_code = EXIT_SUCCESS;
	if (cmd.argc > 2)
		return (ret_exit_msg(sh, EXIT_FAILURE, "exit: too many arguments\n"));
	if (cmd.argc > 1)
	{
		if (is_valid_exit_code(cmd.argv[1]))
			exit_code = (unsigned char)(ft_atoll(cmd.argv[1]) % 256);
		else
		{
			msg_err_3("exit: ", cmd.argv[1], ": numeric argument required");
			exit_code = 2;
		}
	}
	exit_free_with_code(sh, exit_code);
	return (exit_code);
}
