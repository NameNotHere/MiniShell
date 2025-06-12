/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/12 14:35:07 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	ft_strcmp(char *str, char *str1)
{
	while (*str && *str1)
	{
		if (*str++ != *str1++)
			break ;
	}
	if (*str == '\0' && *str1 == '\0')
		return (0);
	return (1);
} // unusual way but my tokeniser uses it this way
// was tired

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	char	*string = "cat <<EOF | grep \"pattern\" > output.txt";
	t_token *output = tokenize(string);
	printf("min\n");

	printf("%d \n", (int)sizeof(output));
	while ((int)sizeof(output) >= i)
		printf("%d \n", output[i++].ty);
	return (0);
}
