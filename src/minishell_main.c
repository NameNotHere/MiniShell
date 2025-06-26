/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 15:01:42 by otanovic         ###   ########.fr       */
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
	t_token		*tokens;
	int			err;

	printf("Input string: %s\n", string);
	printf("Expected token count: %d\n", count_tokens(string, 0));
	tokens = tokenize(string, &token_count, &err);
	if (!tokens)
	{
		printf("tokenizer failed with error #%d\n", err);
		return ;
	}
	printf("Actual token count: %d\n", token_count);
	i = -1;
	while (token_count > ++i)
		printf("%2d %12s  %s \n",
			tokens[i].ty,
			get_token_name(tokens[i].ty),
			tokens[i].word);
	test_build_ast(tokens);
	free_tokens(tokens, token_count);
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
			safe_free_string(&line);
			break ;
		}
		if (ft_strlen(line))
			test_parsing(line);
		safe_free_string(&line);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	if (argc == 2 && (ft_strncmp(argv[1], "-i", 2) == 0))
// 		return (run_pipex_interactive(argv[0], envp));
// 	else
// 		return (run_pipex_once(argc, argv, envp));
// }
