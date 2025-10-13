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

int	handle_export_name_only(t_msh **sh, char *arg)
{
	int	i;

	if (!is_valid_var_name(arg))
	{
		msg_err_3("export: `", arg, "': not a valid identifier");
		(*sh)->exit_code = 1;
		return (EXIT_FAILURE);
	}
	i = search_name(arg, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, arg, "");
	return (EXIT_SUCCESS);
}

int	handle_export_assignment(t_msh **sh, char *arg, char *equals_pos)
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
		return (ft_env((*sh), 1));
	(*sh)->exit_code = EXIT_SUCCESS;
	arg_idx = 1;
	while (arg_idx < cmd.argc)
	{
		// Check for invalid options (starts with --)
		if (cmd.argv[arg_idx][0] == '-' && cmd.argv[arg_idx][1] == '-')
		{
			msg_err_3("export: `", cmd.argv[arg_idx], "': not a valid identifier");
			(*sh)->exit_code = 2;
			arg_idx++;
			continue;
		}
		equals_pos = ft_strchr(cmd.argv[arg_idx], '=');
		if (!equals_pos)
			result = handle_export_name_only(sh, cmd.argv[arg_idx]);
		else
			result = handle_export_assignment(sh, cmd.argv[arg_idx], equals_pos);
		if (result == EXIT_FAILURE)
			(*sh)->exit_code = 1;
		arg_idx++;
	}
	return ((*sh)->exit_code);
}
