/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/23 14:59:22 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

/*
TODO: REMOVE TEST BEFORE EVALUATION
*/
void	test_build_ast(t_token *tokens)
{
	t_ast_node	*ast;


	ast = make_ast_node(NODE_UNKNOWN);
	if (!ast)
		return ;
	build_ast(ast, tokens);
	print_ast(ast);
	free_ast(ast);
	return ;
}

/*
TODO: REMOVE TEST BEFORE EVALUATION
This test runs every time a line is sent to readline.
*/
void	test_parsing(char *string)
{
	int			i;
	int			token_count;
	t_token		*output;

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
	test_build_ast(output);
	free_tokens(output);
	return ;
}

int	main(void)
{
	char	*line;

	while (1)
	{
		if (!readline_on_tty(MINISHELL_PROMPT, &line))
			return (ft_putstr_fd("terminal (tty) not available\n", \
				STDERR_FILENO), EXIT_FAILURE);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			safe_free(&line);
			break ;
		}
		if (ft_strlen(line))
			test_parsing(line);
		safe_free(&line);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}
