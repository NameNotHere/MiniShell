/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/26 14:12:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <stdio.h>

// everything is a heredoc but properly split in size but not properly displayed
int	main(void)
{
	int		i;
	int		token_count;
	t_token	*tokens;
	int 	err;
	// char *string = "VAR=hello || && echo \"Double quotes\" && echo 'Single quotes' && echo Unquoted | grep quotes > out.txt && cat < out.txt && echo $VAR && echo \\$HOME && cd .. && pwd && ls *.c | wc -l && echo done && cat << EOF\nThis is a heredoc\n$HOME should not expand\nEOF\n";
	char *string = "VAR 55hello  ec\"ho  66 \"Double quotes\"  echo \'Single quotes\'  echo Unquoted  grep quotes  out.txt cat out.txt  echo $VAR echo \\$HOME && cd pwd  ls *.c  wc -l  echo done cat EOF\nThis is a heredoc\n$HOME should not expand\nEOF\n";

	printf("Input string: %s\n", string);
	printf("Expected token count: %d\n", count_tokens(string));
	tokens = tokenize(string, &token_count, &err);
	if (!tokens)
	{
		printf("tokenizer failed with error #%d\n", err);
		return (err);
	}
	printf("Actual token count: %d\n", token_count);
	i = -1;
	while (token_count > ++i)
		printf("%2d %12s  %s \n",
			tokens[i].ty,
			get_token_name(tokens[i].ty),
			tokens[i].word);
	free_tokens(tokens, token_count);
	return (0);
}
