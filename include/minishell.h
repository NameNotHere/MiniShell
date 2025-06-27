/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/27 10:39:01 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"
# include "minishell_parser.h"

/*
	\033[96m meanss cyan
	colors need to be wrapped in \001 and \002 for readline to calculate prompt
	lenght correctly
	prompt is: star+arrow(cyan) followed by user input (default term color)
*/
# define MINISHELL_PROMPT "\001\033[96m\002✶➜\001\033[0m\002 "

/*
TODO: remove comments
Removed from the pipex struct:
typedef struct s_pipex
{
	int		argc; -> TODO: check if ever needed?
	char	**argv;
	char	**envp;
	char	***cmd_arg; -> REMOVED -> ast cmd nodes have them
	bool	*cmd_not_found; -> REMOVED -> ast cmd nodes SHOULD have them
	char	**cmd_path; -> REMOVED -> ast cmd nodes should have them
	char	**path_dirs;
	size_t	cmd_offset; -> REMOVED (NOT NEEDED) -> this is for pipex logic
	size_t	cmd_total; -> REMOVED (NOT NEEDED) -> this is for pipex logic
	int		fdin; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		fdout; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		outfile_flags; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	bool	hdoc; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	char	*infile; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	char	*outfile; -> REMOVED -> ast cmd nodes SHOULD have them (redir)
	int		exit_code;
}	t_pipex;
*/
typedef struct s_msh
{
	t_ast	*ast;
	char	**argv;
	char	**envp;
	char	**path_dirs;
	char	*line;
	int		argc;
	int		exit_code;
}	t_msh;

// minishell_main.c

int		initialize_minishell(t_msh *sh, int argc, char **argv, char **envp);

// exec/process.c
// TODO: remove debug functions before eval.

void	debug_print_one_redir(t_redir *redir);

void	execute_ast_node(t_msh *sh, t_ast *node, bool from_pipe);

// exec/lookup_cmd_fullpath.c

void	lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// exec/utils/utils_path.c

char	*make_cmd_full_path(const char *dir, const char *cmd);

char	*get_valid_cmd_full_path(char **path_dirs, char *cmd);

char	*get_path_from_env(char **envp);

// utils/utils_free.c

void	safe_free_string(char **ptr);

void	safe_free_2d_string(char ***ptr);

void	safe_free_3d_string(char ****ptr);

void	safe_free_bool(bool **ptr);

// utils/utils_readine

bool	readline_on_tty(const char *prompt, char **line);

#endif