
#include "minishell_parser.h"

bool	has_quotes(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '"' || *str == '\'')
			return (true);
		str++;
	}
	return (false);
}

bool	has_single_quotes(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '\'')
			return (true);
		str++;
	}
	return (false);
}