/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/12 16:02:47 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// unusual way but my tokeniser uses it this way
int	ft_strcmp(char *str, char *str1)
{
	while (*str && (*str == *str1))
	{
		str++;
		str1++;
	}
	return ((unsigned char)*str - (unsigned char)*str1);
}

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	int	token_count;
	char	*string = "cat <<EOF | grep \"pattern\" > output.txt";
	t_token *output = tokenize(string, &token_count);

	printf("%d\n", ft_strcmp("str", "strpp"));
	while (token_count > i)
		printf("%s \n", output[i++].word);
	return (0);
}
