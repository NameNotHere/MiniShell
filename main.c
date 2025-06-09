/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/09 15:06:17 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	strcmp(char *str, char *str1)
{
	char	*s;
	char	*s2;

	while (*s && *s1)
	{
		if (*s++ != *s1++)
			break ;
	}
	if (*s == '\0' && *s1 == '\0')
		return (1);
	return (0);
}

void	main()
{
	char	*string = "cat <<EOF | grep "pattern" > output.txt";

}
