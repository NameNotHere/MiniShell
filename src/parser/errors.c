
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

void *ft_malloc(size_t amount, size_t size)
{
    void    *mal;

    mal = malloc(size * amount);
    if (!mal)
        printf("malloc failed\n");
    return (mal);
}