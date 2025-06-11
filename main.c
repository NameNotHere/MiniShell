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
		return (1);
	return (0);
}

int	main()
{
	int i = 0;
	char	*string = "cat <<EOF | grep \"pattern\" > output.txt";
	t_token *output = tokenize(string);
	printf("ok main\n");
	while ((int)sizeof(output) >= i)
		printf("%s\n", output[i++].word);
	return (0);
}
