/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 18:24:55 by tda-roch         ###   ########.fr       */
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
