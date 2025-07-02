
#include "minishell.h"

char	*cd(char *path, char *new_path)
{
	char	*output;
	char	*temp;

	if (!new_path)
		return (path);
	if (!path)
		path = ".";
	output = ft_strjoin(path, "/");
	temp = output;
	output = ft_strjoin(output, new_path);
	free(temp);
	if (access(output, F_OK) == 0)
	{
		free(path); // not sure if I need this
		return (output);
	}
	else
		printf("access error\n");
	return (path);
}

// printf is allowed. but we cannot print char array like that
void	pwd(char **path_dirs)
{
	// printf("%s\n", path_dirs);
	(void)path_dirs;
}

// ahahahahaah this is recursive... exit was calling itself.
void	minishell_exit(void)
{
	exit (0);
}
