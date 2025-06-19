/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/19 01:57:54 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <stdio.h>

// everything is a heredoc but properly split in size but not properly displayed
int	main(void)
{
	int		i;
	int		token_count;
	t_token	*output;
	char	*string = "~/po ~./opop *.c ~/opop/oo cat 'hello world' << 99 a && EO|F \"pop\" ../pop/o ./moo/txt $doodoo | grep \"pattern\" > export env unset pipi cd output.txt ./mimi";

	printf("Input string: %s\n", string);
	printf("Expected token count: %d\n", count_tokens(string));
	output = tokenize(string, &token_count);
	printf("Actual token count: %d\n", token_count);
	i = -1;
	while (token_count > ++i)
		printf("%2d %12s  %s \n",
			output[i].ty,
			get_token_name(output[i].ty),
			output[i].word);
	printf("\nLast part of string: '%s'\n", string + ft_strlen(string) - 30);
	return (0);
}
