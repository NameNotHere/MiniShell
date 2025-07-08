/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:00:18 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/08 17:05:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEBUG_H
# define MINISHELL_DEBUG_H

# include <stdio.h>
# include <stdarg.h>

# define DEBUG_TOKENIZE false
# define DEBUG_AST false
# define DEBUG_MINISHELL false

// forward declarations:
typedef struct s_ast	t_ast;
typedef struct s_msh	t_msh;
typedef struct s_redir	t_redir;
typedef struct s_token	t_token;
typedef enum e_redir_ty	t_redir_ty;

// debug/utils_debug.c
void	t_print(const char *str, ...);
void	a_print(const char *str, ...);
void	d_print(const char *str, ...);
void	temp_print(const char *str, ...);

// debug/ast_print.c
char	*get_redir_symbol(t_redir_ty ty);
void	print_ast(t_ast *root);
void	print_ast_cmd(t_ast *node);
void	print_ast_node(t_ast *node, int depth);
int		print_build_ast(t_msh *sh, t_ast *ast, t_token *tokens);

// debug/process_debug.c
void	debug_print_one_redir(t_redir *redir);

#endif