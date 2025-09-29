/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/23 20:47:44 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

static void	ft_echo_arg(char **argv, int *arg_i, bool *first_content)
{
	int		content_i;

	if (!*first_content)
		write(1, " ", 1);
	*first_content = false;
	content_i = 0;
	while (argv[*arg_i][content_i])
	{
		if (argv[*arg_i][content_i] != '\\')
			write(1, &argv[*arg_i][content_i], 1);
		content_i++;
	}
	(*arg_i)++;
}

int	ft_echo(char **argv, int argc)
{
	int		arg_i;
	bool	first_content;
	bool	has_n_flag;

	first_content = true;
	has_n_flag = false;
	arg_i = 1;
	while (arg_i < argc && argv[arg_i] && is_valid_n_flag(argv[arg_i]))
	{
		has_n_flag = true;
		arg_i++;
	}
	while (arg_i < argc && argv[arg_i])
		ft_echo_arg(argv, &arg_i, &first_content);
	if (!has_n_flag)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
