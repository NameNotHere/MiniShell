
#include <minishell.h>

void    is_closed(char *str, int i, char quote)
{
    while (str[i])
    {
        if (str[i] == quote)
            return ;
        i++;
    }
    printf("unclosed quotes\n");
}