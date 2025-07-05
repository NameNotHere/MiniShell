/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/05 12:10:32 by tda-roch         ###   ########.fr       */
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
	\033[96m = cyan
	colors need to be wrapped in \001 and \002
		for readline to calculate prompt lenght correctly
	prompt is: star+arrow(cyan)
	user input has default term color
*/
# define MINISHELL_PROMPT "\001\033[96m\002✶➜\001\033[0m\002 "

typedef enum e_err_code
{
	E_INVALID_REDIR = 200,
	E_MULTIPLE_CMD
}	t_err_code;

# define E_INVALID_REDIR_MSG "syntax error: invalid redirection, missing string"
# define E_MULTIPLE_MSG "syntax error: multiple commands"
// minishell_main.c

int		initialize_minishell(t_msh *sh, char **envp);

void	free_everything(t_msh *sh);

int		minishell_mainloop(t_msh *sh);

// minishell_line.c

int		parse_line_and_execute_ast(t_msh *sh);

int		parse_line_to_ast(t_msh *sh, t_ast *ast, char *string);

// exec/process.c
// TODO: remove debug functions before eval.

void	debug_print_one_redir(t_redir *redir);

void	execute_ast_node(t_msh *sh, t_ast *node, bool from_pipe);

// exec/lookup_cmd_fullpath.c

int		lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// utils/utils_char.c

int		ft_isalnum_underscore(int c);

int		ft_is_singlequote(int c);

int		ft_is_doublequote(int c);

int		ft_is_quote(int c);

// utils/utils_string_array.c

char	**copy_string_array(char **strings);

int		ft_strlen_array(char **array);

// utils/utils_env.c

bool	is_var_in_env(t_msh *sh, char *var, int *envp_index);

char	*get_env_value(t_msh *sh, char *var_name, int envp_index);

// utils/utils_free.c

void	safe_free_string(char **ptr);

void	safe_free_2d_string(char ***ptr);

void	safe_free_3d_string(char ****ptr);

void	safe_free_bool(bool **ptr);

// utils/utils_path.c

char	*make_cmd_full_path(const char *dir, const char *cmd);

char	*get_valid_cmd_full_path(char **path_dirs, char *cmd);

char	*get_path_from_env(char **envp);

// utils/utils_readine

bool	readline_on_tty(const char *prompt, char **line);

// utils/utils_string.c

char	*get_empty_string(void);

// filenavs.c

char	*cd(char *path, char *new_path);

void	pwd(char **path_dirs);

void	minishell_exit(void);

// signals/signal.c

void	ctrl_c(int sig);

#endif