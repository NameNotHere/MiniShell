/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/29 04:05:53 by tda-roch         ###   ########.fr       */
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
	if (cmd.argc > 2)
	{
		put_stderr("exit: too many arguments\n");
		sh->exit_code = EXIT_FAILURE;
		return (sh->exit_code);
	}
	if (cmd.argc > 1)
	{
		if (is_valid_exit_code(cmd.argv[1]))
			sh->exit_code = ft_atoi(cmd.argv[1]);
		else
		{
			put_stderr_3("exit: ", cmd.argv[1], ": numeric argument required");
			sh->exit_code = 2;
		}
	}
	exit_free_with_code(sh, sh->exit_code);
	return (sh->exit_code);
}

/*
	Validates variable names.
	Returns
		true if valid
		false if invalid
	NOTES:
	First character must be letter or underscore
	Rest can be letters, digits, or underscores
*/
bool	is_valid_var_name(char *name)
{
	int	i;

	if (!name || !*name)
		return (false);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (false);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/*
	Handles export in two formats:
		export NAME=VALUE  (with assignments)
		or
		export NAME  (without assignment)
	but only the first format does something.

	NOTES:
	Since current implementation does not support shell variables (like adding
	variables with direct assignment like NAME=VALUE without the export keyword)

	Direct assignment without export keyworkd like NAME=VALUE will be dealt like
	invalid command.

	Check if it's a valid variable name even without assignment

	Valid variable name is checked in both formats, but assignment only occurs
	with the assignment format.
*/
int	ft_export(t_msh **sh, t_cmd cmd)
{
	char	*name;
	char	*value;
	char	*equals_pos;
	int		i;

	if (!cmd.argv[1])
		return (EXIT_SUCCESS);
	equals_pos = ft_strchr(cmd.argv[1], '=');
	if (!equals_pos)
	{
		if (!is_valid_var_name(cmd.argv[1]))
		{
			put_stderr_3("export: `", cmd.argv[1], "': not a valid identifier");
			return (EXIT_FAILURE);
		}
		return (EXIT_SUCCESS);
	}
	*equals_pos = '\0';
	name = cmd.argv[1];
	value = equals_pos + 1;
	if (!is_valid_var_name(name))
	{
		put_stderr_3("export: `", cmd.argv[1], "': not a valid identifier");
		return (EXIT_FAILURE);
	}
	i = search_name(name, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, name, value);
	else
		change_env_value(name, value, &(*sh)->envp);
	if (search_name(name, (*sh)->envp) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
