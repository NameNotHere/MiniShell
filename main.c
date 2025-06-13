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

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	int	token_count;
	char	*string = "cat << 99 a EOF | grep \"pattern\" > output.txt";
	// char	*string = "cat EOF grep \"pattern\"  output txt";

	t_token *output = tokenize(string, &token_count);
	printf("main\n");
	while (token_count > i)
	{
		printf("%d %s \n", output[i].ty, output[i].word);
		i++;
	}
	return (0);
}
