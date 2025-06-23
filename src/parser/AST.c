/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 02:59:59 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/23 03:01:26 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell_parser.h"

/*
AST DATASTRUCTURE ADDED TO minishell.h
*/

t_redir_type	get_redir_type(t_token_type ty)
{
	if (ty == TOKEN_INPUT)
		return (REDIR_INPUT);
	if (ty == TOKEN_OUTPUT)
		return (REDIR_OUTPUT);
	if (ty == TOKEN_APPEND)
		return (REDIR_APPEND);
	if (ty == TOKEN_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_UNKNOWN);
}

// TODO: HANDLE ALLOC ERRORS for ft_calloc & ft_strdup
// TODO: maybe: check valid ast and word
void	add_redir_node(t_ast_node *ast, t_token_type token_type, char *word)
{
	t_redir_node	*current_redir;
	t_redir_node	*new_redir;

	new_redir = ft_calloc(1, sizeof(t_redir_node));
	if (!new_redir)
		return ;
	new_redir->string = ft_strdup(word);
	if (!new_redir->string)
	{
		free(new_redir);
		return ;
	}
	new_redir->type = get_redir_type(token_type);
	printf("__redir: ty %d : %s\n", new_redir->type, word);
	if (!ast->data.cmd.redir)
		ast->data.cmd.redir = new_redir;
	else
	{
		current_redir = ast->data.cmd.redir;
		while (current_redir->next)
			current_redir = current_redir->next;
		current_redir->next = new_redir;
	}
}

t_ast_node	*make_ast_node(t_node_type type)
{
	t_ast_node	*new_node;

	new_node = ft_calloc(1, sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->nty = type;
	return (new_node);
}
/*
TODO: remove printfs, add error handling
*/
void	parse_redir(t_ast_node *ast, t_token *tokens, int *start, int *end)
{
	int				i;
	int				cmd_count;
	bool			scan_redir;
	int				last_cmd_token;

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
			add_redir_node(ast, tokens[i].ty, tokens[i+1].word);
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
	i = -1;
	printf(" :: argv -> ");
	while (argv[++i] != NULL)
		printf("|%s", argv[i]);
	printf("|\n");
	return (argv);
}

/*
	- NOT validate things that are supposed to fail in execve (ex: invalid path)
	- return clean AST node
	TODO: TOKEN # MUST COINCIDE WITH ARGV #! (so inside cmd, space or quote
	separated words MUST be equal to token number OR there must be a way to
	convert tokens to unify the separated ones (so no loss of information about
	space separation is allowed, or argv is not reacreatable.))
*/
void	parse_cmd(t_ast_node *ast, t_token *tokens, int start, int end)
{
	printf("cmd node->ADD\n" \
		"	start cmd tk: %d, end cmd tk: %d\n",
		start,
		end);
	// 3. extract and process redir tokens into redir nodes -> done below

	ast->nty = NODE_CMD;
	parse_redir(ast, tokens, &start, &end);
	printf("		$ command is:%s, ends with %s\n", tokens[start].word,
		tokens[end].word);
	// TODO:
	// 1. validade syntax,
	// 2. validate built in options (check: do we still run other commands?)
	// 4. remove quotes if needed & expand vars,
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

	ast->data.pipe.left = make_ast_node(NODE_CMD);
	parse_cmd(ast->data.pipe.left, tokens, start, end - 1);
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
	ast->data.pipe.right = make_ast_node(NODE_CMD);
	parse_cmd(ast->data.pipe.right, tokens, start, *i);
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
		return ;
	i = -1;
	while (tokens[++i].word)
		scan_pipe(ast, tokens, &i);
	return ;
}
