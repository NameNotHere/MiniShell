
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
	int		env_len;
	char	*new_var;
	char	**resized_env;
	char	*tmp;

	env_len = envp_len(*envp);
	resized_env = realloc(*envp, sizeof(char *) * (env_len + 2));
	if (!resized_env)
		return (EXIT_FAILURE);

	*envp = resized_env;
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (EXIT_FAILURE);
	new_var = ft_strjoin(tmp, value);
	free(tmp);
	if (!new_var)
		return (EXIT_FAILURE);

	(*envp)[env_len] = new_var;
	(*envp)[env_len + 1] = NULL;

	return (EXIT_SUCCESS);
}

