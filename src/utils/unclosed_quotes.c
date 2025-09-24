
#include "minishell.h"

int	unclosed_quotes(const char *line)
{
    bool    single;
    bool    dbl;

    single = false;
    dbl = false;
    while (*line)
    {
        if (*line == '\'' && !dbl)
            single = !single;
        else if (*line == '"' && !single)
            dbl = !dbl;
        line++;
    }
    return (single || dbl);
}