/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/22 02:45:14 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell_parser.h"

/*
AST DATASTRUCTURE ADDED TO minishell.h
*/

void	parse_redir(t_token *tokens, int *start, int *end, t_redir_node *redir)
{
	int				i;
	int				cmd_count;
	bool			scan_redir;
	int				last_cmd_token;
	t_redir_node	*new;

	cmd_count = 0;
	scan_redir = true;
	last_cmd_token = -1;
	i = *start - 1;
	while (++i <= *end)
	{
		if (tokens[i].ty == TOKEN_INPUT || tokens[i].ty == TOKEN_HEREDOC \
			|| tokens[i].ty == TOKEN_APPEND || tokens[i].ty == TOKEN_OUTPUT)
		{
			if (i == *end)
				printf(" ***ERROR*** " \
					"invalid redirection, needs a file or delimiter\n");
			scan_redir = true;
			if (redir->string == NULL)
				new = redir;
			else
			{
				redir->next = ft_calloc(1, sizeof(t_redir_node));
				new = redir->next;
			}
			new->string = ft_strdup(tokens[i+1].word); //guard for failure
			if (tokens[i].ty == TOKEN_INPUT)
				new->type = REDIR_INPUT;
			if (tokens[i].ty == TOKEN_OUTPUT)
				new->type = REDIR_OUTPUT;
			if (tokens[i].ty == TOKEN_APPEND)
				new->type = REDIR_APPEND;
			if (tokens[i].ty == TOKEN_HEREDOC)
				new->type = REDIR_HEREDOC;
			printf("__redir: ty %d : %s\n", tokens[i].ty, tokens[i+1].word);
			i++;
		}
		else
		{
			if (scan_redir)
			{
				cmd_count++;
				scan_redir = false;
				if (cmd_count > 1)
					printf(" *** ERROR *** "\
					"invalid syntax, multiple commands!\n");
				else
					*start = i;
			}
			last_cmd_token = i;
		}
	}
	if (last_cmd_token >= 0 && last_cmd_token < *end)
	{
		printf("Updated cmd end from %d to %d\n", *end, last_cmd_token);
		*end = last_cmd_token;
	}
	if (cmd_count != 1)
		printf("  **** ERROR *** no command or multiple commands\n");
}


char	**token_words_to_argv(t_token *tokens, int start, int end)
{
	int		i;
	char	**argv;

	argv = ft_calloc(end - start + 2, sizeof(char *));
	i = -1;
	while (++i + start <= end)
		argv[i] = ft_strdup(tokens[i + start].word);
	argv[i] = NULL; // redundant with ft_calloc, but safe nonetheless
	i = -1;
	printf(" :: argv -> ");
	while (argv[++i] != NULL)
		printf("|%s", argv[i]);
	printf("|\n");
	return (argv);
}

/*
CMD only tokens sent here (knowing start and end of cmd tokens):
	- 1st - will extract command name (with or without path)
	- process arguments (expand variables, remove quotes)
	- detect redirections (extract all redir tokens from CMD parsing and use
	them to add redir_in_node or redir_out_node)
	- validate most things (syntax errors, invalid built in params)
	- NOT validate things that are supposed to fail in execve (ex: invalid path)
	- return clean AST node
	TODO: TOKEN # MUST COINCIDE WITH ARGV #! (so inside cmd, space or quote
	separated words MUST be equal to token number OR there must be a way to
	convert tokens to unify the separated ones (so no loss of information about
	space separation is allowed, or argv is not reacreatable.))
*/
void	parse_cmd(t_ast_node *ast, t_token *tokens, int start, int end)
{
	t_redir_node	redir;

	ast->data.cmd.redir = ft_calloc(1, sizeof(t_redir_node));
	redir = *(ast->data.cmd.redir);

	printf("cmd node->ADD\n" \
		"	start cmd tk: %d, end cmd tk: %d\n",
		start,
		end);
	// 3. extract and process redir tokens into redir nodes -> done below
	parse_redir(tokens, &start, &end, &redir);
	printf("		$ command is:%s, ends with %s\n", tokens[start].word,
		tokens[end].word);
	// TODO:
	// 1. validade syntax,
	// 2. validate options,
	// 4. expand vars,
	// 5. cleanup
	// 6. then build argv. (done below)
	ast->data.cmd.built_in = false;
	ast->data.cmd.argv = token_words_to_argv(tokens, start, end);
	return ;
}

void	parse_pipe(t_ast_node *ast, t_token *tokens, int start, int end)
{
	if (!(tokens && tokens[0].word))
		return ;
	printf("pipe node->ADD\n");
	printf("	start pipe tk: %d, end pipe tk: %d\n",
		start,
		end);
	if (ast == NULL)
		printf("### ast not initialized yet, maloc it here?\n");

	parse_cmd(ast, tokens, start, end - 1);
	return ;
}

/*scanning if pipe is found, if yes, call parsing with start/end */
void	scan_pipe(t_ast_node *ast, t_token *tokens, int *i)
{
	int	start;

	start = *i;
	if (tokens[start].word == NULL)
		return ;
	while (tokens[*i].word)
	{
		if (tokens[*i].ty == TOKEN_PIPE)
		{
			parse_pipe(ast, tokens, start, *i);
			return ;
		}
		(*i)++;
	}
	(*i)--;
	parse_cmd(ast, tokens, start, *i);
	return ;
}

/*
creates the ast node pointer then starts scan
keeps scanning while there are tokens in line
--> using iterative instead of recursive approach - safer? probably
*/
void	build_ast(t_ast_node *ast, t_token *tokens)
{
	int			i;

	if (tokens == NULL || tokens[0].word == NULL)
		// return (NULL);
		return ;
	i = -1;
	while (tokens[++i].word)
		scan_pipe(ast, tokens, &i);
	return ;
}
