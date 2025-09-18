/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/18 11:05:02 by tda-roch         ###   ########.fr       */
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
		if (ft_strncmp(envp[i], name, len_name) == 0 &&\
			envp[i][len_name] == '=')
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

int	print_var(int *i, t_cmd *cmd, t_msh sh, int z)
{
	int		start;
	char	*var_value;
	char	*var_name;

	start = ++(*i);
	while (cmd->argv[z][*i] && cmd->argv[z][*i] != ' ')
		(*i)++;
	var_name = ft_strndup(&cmd->argv[z][start], (*i) - start);
	if (!var_name)
		return (EXIT_FAILURE);
	var_value = write_var(sh.envp, var_name);
	if (var_value)
		write(1, var_value, ft_strlen(var_value));
	free(var_name);
	return (EXIT_SUCCESS);
}

int	ft_echo_arg(t_msh *sh, t_cmd *cmd, int *arg_i, bool *first_content)
{
	int		content_i;

	(void)sh;
	if (!*first_content)
		write(1, " ", 1);
	*first_content = false;
	content_i = 0;
	while (cmd->argv[*arg_i][content_i])
	{
		write(1, &cmd->argv[*arg_i][content_i], 1);
		content_i++;
	}
	(*arg_i)++;
	return (EXIT_SUCCESS);
}

int	ft_echo(t_cmd *cmd, t_msh *sh)
{
	int		res;
	int		arg_i;
	bool	first_content;
	bool	has_n_flag;

	res = EXIT_SUCCESS;
	first_content = true;
	has_n_flag = false;
	arg_i = 1;
	while (cmd->argv[arg_i] && ft_strncmp(cmd->argv[arg_i], "-n", 2) == 0)
	{
		has_n_flag = true;
		arg_i++;
	}
	while (cmd->argv[arg_i] && res == EXIT_SUCCESS)
		res = ft_echo_arg(sh, cmd, &arg_i, &first_content);
	if (!has_n_flag)
		write(1, "\n", 1);
	return (res);
}
