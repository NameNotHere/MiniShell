

#include "minishell.h"

bool	is_escaped(const char *str, int i)
{
	int	backslash_count;
	int	j;
	int	ret;

	backslash_count = 0;
	if (i <= 0)
		return (false);
	j = i - 1;
	while (j >= 0 && str[j] == '\\')
	{
		backslash_count++;
		j--;
	}
	ret = backslash_count % 2;
	if (ret != 0)
		return (true);
	else
		return (false);
}
