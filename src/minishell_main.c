/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/27 06:09:20 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>



/*
TODO: REMOVE TEST BEFORE EVALUATION
*/
void	test_build_ast(t_ast *ast, t_token *tokens)
{
	// t_ast	*ast;


	// ast = make_ast_node(NODE_UNKNOWN);
	if (!ast)
		return ;
	build_ast(ast, tokens);
	print_ast(ast);
	// free_ast(ast);
	return ;
}

/*
TODO: REMOVE TEST BEFORE EVALUATION
This test runs every time a line is sent to readline.
*/
void	test_parsing(t_ast *ast, char *string)
{
	int			i;
	int			token_count;
	t_token		*tokens;
	int			err;

	printf("Input string: %s\n", string);
	printf("Expected token count: %d\n", count_tokens(string));
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
	test_build_ast(ast, tokens);
	free_tokens(tokens, token_count);
	return ;
}

/*
TODO: check if regular readline will work fine.
otherwise, get this code back, to run with /dev/tty instead of stdin/stdout:
	// if (!readline_on_tty(MINISHELL_PROMPT, &line))
	// 	return (ft_putstr_fd("terminal (tty) not available\n", \
	// 		STDERR_FILENO), EXIT_FAILURE);
*/
int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;

	if (initialize_minishell(&sh, argc, argv, envp))
		return (EXIT_FAILURE);
	while (1)
	{
		sh.line = readline(MINISHELL_PROMPT);
		if (!sh.line)
			break ;
		if (*sh.line)
			add_history(sh.line);
		if (ft_strncmp(sh.line, "exit", 4) == 0)
		{
			safe_free_string(&sh.line);
			break ;
		}
		if (ft_strlen(sh.line))
		{
			test_parsing(sh.ast, sh.line);
			printf(" *checking command paths*\n\n");
			lookup_all_cmd_fullpaths(&sh, sh.ast);
			// execute_ast(&sh, sh.ast);
		}
		free_ast(sh.ast);
		safe_free_string(&sh.line);
	}
	rl_clear_history();
	return (EXIT_SUCCESS);
}

/*
TODO: handle allocation error for path dirs
*/
int	initialize_minishell(t_msh *sh, int argc, char **argv, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	sh->argc = argc;
	sh->argv = argv;
	sh->envp = envp;
	sh->ast = make_ast_node(NODE_UNKNOWN);
	sh->path_dirs = ft_split(get_path_from_env(envp), ':');
	if (!sh->path_dirs)
	{
		printf("memory allocation failed for path diretories");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
