/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_export.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/01 02:13:46 by tda-roch         ###   ########.fr       */
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
		return (ret_exit_msg(sh, EXIT_FAILURE, "exit: too many arguments\n"));
	if (cmd.argc > 1)
	{
		if (is_valid_exit_code(cmd.argv[1]))
			sh->exit_code = ft_atoi(cmd.argv[1]);
		else
		{
			msg_err_3("exit: ", cmd.argv[1], ": numeric argument required");
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

static int	handle_export_name_only(t_msh **sh, char *arg)
{
	int	i;

	if (!is_valid_var_name(arg))
	{
		msg_err_3("export: `", arg, "': not a valid identifier");
		(*sh)->exit_code = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}
	i = search_name(arg, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, arg, "");
	return (EXIT_SUCCESS);
}

static int	handle_export_assignment(t_msh **sh, char *arg, char *equals_pos)
{
	char	*name;
	char	*value;
	int		i;

	*equals_pos = '\0';
	name = arg;
	value = equals_pos + 1;
	if (!is_valid_var_name(name))
	{
		msg_err_3("export: `", name, "': not a valid identifier");
		*equals_pos = '=';
		return (EXIT_FAILURE);
	}
	i = search_name(name, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, name, value);
	else
		change_env_value(name, value, &(*sh)->envp);
	*equals_pos = '=';
	return (EXIT_SUCCESS);
}

int	ft_export(t_msh **sh, t_cmd cmd)
{
	char	*equals_pos;
	int		arg_idx;
	int		result;

	if (!cmd.argv[1])
		return (EXIT_SUCCESS);
	(*sh)->exit_code = EXIT_SUCCESS;
	arg_idx = 1;
	while (arg_idx < cmd.argc)
	{
		equals_pos = ft_strchr(cmd.argv[arg_idx], '=');
		if (!equals_pos)
			result = handle_export_name_only(sh, cmd.argv[arg_idx]);
		else
			result = handle_export_assignment(sh, cmd.argv[arg_idx], equals_pos);
		if (result == EXIT_FAILURE)
			(*sh)->exit_code = EXIT_FAILURE;
		arg_idx++;
	}
	return ((*sh)->exit_code);
}
