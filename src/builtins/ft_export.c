/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 12:09:12 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/29 17:04:38 by tda-roch         ###   ########.fr       */
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

	if (!name || !*name || name[0] == '?')
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
		msg_err_3("export: `", arg, "': not a valid identifier\n");
		return (EXIT_FAILURE);
	}
	i = search_name(arg, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, arg, "");
	return (EXIT_SUCCESS);
}

// TODO: do we need *name? why not just use arg
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
		msg_err_3("export: `", name, "': not a valid identifier\n");
		*equals_pos = '=';
		return (EXIT_FAILURE);
	}
	i = search_name(name, (*sh)->envp);
	if (i == -1)
		add_env_var(&(*sh)->envp, name, value);
	else
		change_env_value(name, value, &(*sh)->envp);
	if (ft_strcmp(name, "PATH") == 0)
		update_path_dirs(&(*sh)->path_dirs, (*sh)->envp);
	*equals_pos = '=';
	return (EXIT_SUCCESS);
}

int	ft_export(t_msh **sh, t_cmd cmd)
{
	char	*equals_pos;
	int		arg_idx;
	int		exit_code;

	if (!cmd.argv[1])
		return (ft_env((*sh), 1));
	arg_idx = 1;
	while (arg_idx < cmd.argc)
	{
		if (cmd.argv[arg_idx][0] == '-')
		{
			msg_err_3("export: `", cmd.argv[arg_idx],
				"': not a valid identifier\n");
			exit_code = 2;
			arg_idx++;
			continue ;
		}
		equals_pos = ft_strchr(cmd.argv[arg_idx], '=');
		if (!equals_pos)
			exit_code = handle_export_name_only(sh, cmd.argv[arg_idx]);
		else
			exit_code = handle_export_assignment(sh, cmd.argv[arg_idx],
					equals_pos);
		arg_idx++;
	}
	return (exit_code);
}
