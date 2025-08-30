
#include <minishell.h>
#include <sys/stat.h>

int ft_echo(char *str, int with_arg_n)
{
    int len;

    len = ft_strlen(str);
    if (with_arg_n == 1)
    {
        while (*str)
        {
            if (*str != '\n')
                write(1, str, 1);
            str++;
        }
        return (len);
    }
    return (write(1, str, len));
}

int     t_cd(char **envp, char *directory)
{
    struct stat st;
    char        *pwd_value;
    char        *new_path;
    char        *temp;
    int          i;

    i = search_name("PWD", envp);
    pwd_value = envp[i] + length_till_equal(envp[i]) + 1;
    temp = ft_strjoin(pwd_value, "/");
    if (!temp)
        return (ft_echo("Memory error\n", 0));
    new_path = ft_strjoin(temp, directory);
    free(temp);
    if (!new_path)
        return (ft_echo("Memory error\n", 0));
    if (stat(new_path, &st) != 0 || !S_ISDIR(st.st_mode))
    {
        free(new_path);
        return (ft_echo("Invalid Directory\n", 0));
    }
    temp = ft_strjoin("PWD=", new_path);
    free(new_path);
    if (!temp) return ft_echo("Memory error\n", 0);
    free(envp[i]);
    envp[i] = temp;
    return (0);
}


void    ft_pwd(char **envp)
{
    int i;
    int equal;

    i = search_name("PWD", envp);
    equal = length_till_equal(envp[i]) + 1;
    ft_echo(envp[i] + equal, 0);
    write(1, "\n", 1);
}