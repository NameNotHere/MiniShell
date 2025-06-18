/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/18 15:36:38 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	int	token_count;
	char	*string = "~/po ~./opop ~/opop/oo cat << 99 a && EO|F \"pop\" ../pop/o ./moo/txt $doodoo | grep \"pattern\" > export env unset pipi cd output.txt ./mimi";

	t_token *output = tokenize(string, &token_count);
	while (token_count > i)
	{
		printf("%d %s \n", output[i].ty, output[i].word);
		i++;
	}
	return (0);
}
