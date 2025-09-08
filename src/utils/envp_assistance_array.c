
#include "minishell.h"

#include <stdio.h>

int	length_till_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	search_name(char *name, char **envp)
{
	int	i;
	int	len_name;
	int	length_envp;

	i = 0;
	len_name = ft_strlen(name);
	while (envp[i])
	{
		length_envp = length_till_equal(envp[i]);
		if (ft_strncmp(name, envp[i], length_envp) == 0
			&& len_name == length_envp)
			return (i);
		i++;
	}
	return (-1);
}

int	change_env_value(char *name, char *new_value, char **envp)
{
	int		index;
	char	*new_entry;
	char	*tmp;

	index = search_name(name, envp);
	if (index == -1)
		return (EXIT_FAILURE);

	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (EXIT_FAILURE);

	new_entry = ft_strjoin(tmp, new_value);
	free(tmp);
	if (!new_entry)
		return (EXIT_FAILURE);

	envp[index] = new_entry;
	return (EXIT_SUCCESS);
}


int	envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

int	add_env_var(char ***envp, char *name, char *value)
{
	int		env_len;
	char	**resized_env;

	env_len = envp_len(*envp);
	resized_env = realloc(*envp, sizeof(char *) * (env_len + 2));
	if (!resized_env)
		return (EXIT_FAILURE);


	resized_env[env_len] = name;
	resized_env[env_len + 1] = NULL;

	change_env_value(name, value, resized_env);
	*envp = resized_env;
	return (EXIT_SUCCESS);
}
