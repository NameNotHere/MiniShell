/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/18 16:49:55 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo_arg(char **argv, int *arg_i, bool *first_content)
{
	int		content_i;

	if (!*first_content)
		write(1, " ", 1);
	*first_content = false;
	content_i = 0;
	while (argv[*arg_i][content_i])
	{
		write(1, &argv[*arg_i][content_i], 1);
		content_i++;
	}
	(*arg_i)++;
}

int	ft_echo(char **argv)
{
	int		arg_i;
	bool	first_content;
	bool	has_n_flag;

	first_content = true;
	has_n_flag = false;
	arg_i = 1;
	while (argv[arg_i] && ft_strncmp(argv[arg_i], "-n", 2) == 0)
	{
		has_n_flag = true;
		arg_i++;
	}
	while (argv[arg_i])
		ft_echo_arg(argv, &arg_i, &first_content);
	if (!has_n_flag)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
