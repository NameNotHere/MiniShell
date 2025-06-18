
#include "libft.h"

int ft_isminioperator(char *token, int i) {
	if (!token)
		return 0;

	if (ft_strncmp(token + i, ">>", 2) == 0)
		return 2;
	if (ft_strncmp(token + i, "<<", 2) == 0)
		return 2;
	if (ft_strncmp(token + i, ">", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, "<", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, "|", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, "(", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, ")", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, "&", 1) == 0)
		return 1;
	if (ft_strncmp(token + i, ";", 1) == 0)
		return 1;
	return 0;
}
