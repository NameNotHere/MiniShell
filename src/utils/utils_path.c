/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:18:36 by tda-roch          #+#    #+#             */
/*   Updated: 2025/08/28 13:42:57 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

// Creates a full command path by concatenating the directory
// and command.
// Returns created full path if successful, or NULL if not.
// Caller is responsible for freeing the returned string.
// The path is not checked for validity or existence.
// Caller needs to check if dir and cmd are not NULL nor "\0".
char	*make_cmd_full_path(const char *dir, const char *cmd)
{
	int		add_slash;
	int		len;
	char	*full;
	int		i;
	int		cmd_i;

	add_slash = (dir[ft_strlen(dir) - 1] != '/');
	len = ft_strlen(dir) + ft_strlen(cmd) + add_slash + 1;
	full = malloc(len * sizeof(char));
	if (!full)
		return (NULL);
	i = 0;
	cmd_i = 0;
	while (dir[i])
	{
		full[i] = dir[i];
		i++;
	}
	if (add_slash)
		full[i++] = '/';
	while (cmd[cmd_i])
		full[i++] = cmd[cmd_i++];
	full[i] = '\0';
	return (full);
}

// TODO: replace ~/ with home folder.
// TODO: replace ./ with PWD?
// TODO: use chdir(current folder) so processes know where they are 
// (like access, to be able to use relative paths)
// Gets a full command path by checking concatenations of path
// directories with command, and checking if the full path exists
// and is executable.
// Also checks if the command contains a '/' character, in which case
// it is treated as a full path.
// Returns full path if found, or NULL if not found.
// Caller is responsible for freeing the returned string.
// TODO: handle ~/ (I had this here, broken if (ft_strncmp(cmd, "~/", 2) == 0))
char	*get_valid_cmd_full_path(char **path_dirs, char *cmd)
{
	int		i;
	char	*full_path;

	if (!path_dirs || !cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	i = 0;
	while (path_dirs[i])
	{
		full_path = make_cmd_full_path(path_dirs[i], cmd);
		i++;
		if (!full_path)
			continue ;
		if (!access(full_path, X_OK))
			return (full_path);
		safe_free_string(&full_path);
	}
	return (NULL);
}

// Retrieves the PATH variable from the environment variables.
// Returns a pointer to the value of PATH (the part after "PATH=").
char	*get_path_from_env(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}
