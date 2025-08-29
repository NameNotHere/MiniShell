
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

	i = 0;
	len_name = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(name, envp[i], length_till_equal(envp[i])) == 0
			&& len_name == length_till_equal(envp[i]))
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
		return (-1);

	len_name = ft_strlen(name);
	len_value = ft_strlen(new_value);
	new_entry = malloc(len_name + len_value + 2);
	if (!new_entry)
		return (-1);

	ft_strcpy(new_entry, name);
	new_entry[len_name] = '=';
	ft_strcpy(new_entry + len_name + 1, new_value);
	free(envp[index]);

	envp[index] = new_entry;

	return (0);
}

int	envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

// make sure this works I cut a lot
int	add_env_var(char ***envp, char *name, char *value)
{
	char	**new_envp;
	char	*new_entry;
	int		name_len;
	int		i;

	new_envp = malloc(sizeof(char *) * (envp_len(*envp) + 2));
	i = 0;
	while (i < envp_len(*envp) - 1)
		new_envp[++i] = (*envp)[i];
	name_len = (int)ft_strlen(name);
	new_entry = malloc(name_len + (int)ft_strlen(value) + 2);
	if (!new_entry)
		return (free(new_envp), -1);
	ft_strcpy(new_entry, name);
	new_entry[name_len] = '=';
	ft_strcpy(new_entry + name_len + 1, value);
	new_envp[i] = new_entry;
	new_envp[i + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}
