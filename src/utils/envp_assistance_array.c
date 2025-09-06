
#include "minishell.h"

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
	int		len_name;
	int		len_value;

	index = search_name(name, envp);
	if (index == -1)
		return (EXIT_FAILURE);

	len_name = ft_strlen(name);
	len_value = ft_strlen(new_value);
	new_entry = malloc(len_name + len_value + 2);
	if (!new_entry)
		return (-1);

	ft_strlcpy(new_entry, name, len_name);
	new_entry[len_name] = '=';
	ft_strlcpy(new_entry + len_name + 1, new_value, len_value);
	free(envp[index]);
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
	char	**new_envp;
	char	*new_entry;
	int		name_len;
	int		value_len;
	int		i;

	new_envp = malloc(sizeof(char *) * (envp_len(*envp) + 2));
	if (!new_envp)
		return (EXIT_FAILURE);
	i = 0;
	while (i < envp_len(*envp))
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_entry = malloc(name_len + value_len + 2);
	if (!new_entry)
		return (free(new_envp), EXIT_FAILURE);
	ft_strlcpy(new_entry, name, name_len + 1);
	new_entry[name_len] = '=';
	ft_strlcpy(new_entry + name_len + 1, value, value_len + 1);
	new_envp[i++] = new_entry;
	new_envp[i] = NULL;
	free(*envp);
	*envp = new_envp;
	return (EXIT_SUCCESS);
}
