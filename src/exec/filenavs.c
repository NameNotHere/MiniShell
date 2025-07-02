
#include "minishell.h"

char    *cd(char *path, char *new_path)
{
    char    *output;
    char    *temp;

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
        ft_printf("access error\n");
    return (path);
}

void    pwd(char **path_dirs)
{
    ft_printf("%s\n", path_dirs); // need to add prtinf
}

// ahahahahaah
void    exit()
{
    exit(0);
}