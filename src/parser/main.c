/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/19 10:04:00 by tda-roch         ###   ########.fr       */
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
	// char *string = "VAR=hello || && echo \"Double quotes\" && echo 'Single quotes' && echo Unquoted | grep quotes > out.txt && cat < out.txt && echo $VAR && echo \\$HOME && cd .. && pwd && ls *.c | wc -l && echo done && cat << EOF\nThis is a heredoc\n$HOME should not expand\nEOF\n";
	char *string = "VAR hello  echo \"Double quotes\"  echo 'Single quotes'  echo Unquoted  grep quotes  out.txt cat out.txt  echo $VAR echo \\$HOME && cd pwd  ls *.c  wc -l  echo done cat EOF\nThis is a heredoc\n$HOME should not expand\nEOF\n";

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
	free_tokens(output, token_count);
	return (0);
}
