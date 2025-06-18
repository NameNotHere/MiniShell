/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/18 22:25:05 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <stdio.h>

// everything is a heredoc but properly split in size but not properly displayed
int	main()
{
	int i = 0;
	int	token_count;
	char	*string = "~/po ~./opop *.c ~/opop/oo cat << 99 a && EO|F \"pop\" ../pop/o ./moo/txt $doodoo | grep \"pattern\" > export env unset pipi cd output.txt ./mimi";

	t_token *output = tokenize(string, &token_count);
	while (token_count > i)
	{
		printf("%d %s \n", output[i].ty, output[i].word);
		i++;
	}
	return (0);
}
