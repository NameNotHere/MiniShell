/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/09 01:29:40 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_parser.h"
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


/*
	\033[96m = cyan
	colors need to be wrapped in \001 and \002
		for readline to calculate prompt lenght correctly
	prompt is: star+arrow(cyan)
	user input has default term color
*/
# define MINISHELL_PROMPT "\001\033[96m\002✶➜\001\033[0m\002 "

// 0644: user can read/write, others can read. reasonable/safe setting.
# define OUTPUT_PERMISSIONS 0644

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

// exec/execute.c
int		execute_ast_node(t_msh *sh, t_ast *node, int fd_in, int fd_out);
int		execute_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out);

// exec/execute_cmd.c
int		execute_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out);

// exec/execute_cmd_redir.c
// void	execute_redirection_in(t_msh *sh, t_redir *redir);
// void	execute_redirection_out(t_msh *sh, t_redir *redir);
void	execute_redirection(t_msh *sh, t_redir *redir);

// exec/execute_cmd_redir_open.c
// int		failed_open_to_null(t_msh *sh, char *filename, int o_flag);
int		open_input_redirection(t_msh *sh, char *filename);
int		open_output_redirection(t_msh *sh, char *filename);
int		open_append_redirection(t_msh *sh, char *filename);

// exec/lookup_cmd_fullpath.c
int		lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// utils/utils_char.c
int		ft_isalnum_underscore(int c);
int		ft_is_singlequote(int c);
int		ft_is_doublequote(int c);
int		ft_is_quote(int c);

// utils/utils_dup2.c
void	try_dup2_stdout(t_msh *sh, int *fd_in, int *fd_out);
void	try_dup2_stdin(t_msh *sh, int *fd_in, int *fd_out);
void	try_dup2(t_msh *sh, int *fd_in, int *fd_out);

// utils/utils_env.c
bool	is_var_in_env(t_msh *sh, char *var, int *envp_index);
char	*get_env_value(t_msh *sh, char *var_name, int envp_index);

// utils/utils_error.c
void	put_stderr(const char *error);
void	put_stderr_2(const char *str1, const char *str2);
void	put_stderr_3(const char *str1, const char *str2, const char *str3);

// utils/utils/exit.c
void	close_fds_exit_error_free(t_msh *sh, const char *error, int *fd_in,
			int *fd_out);
void	exit_error_free(t_msh *sh, const char *error);
void	exit_free_with_code(t_msh *sh, int exit_code);
int		handle_execute_command_errors(t_msh *sh, t_cmd *cmd);
// utils/utils_fd.c
void	safe_close_fd_in(int *fd_in);
void	safe_close_fd_out(int *fd_out);
void	safe_close_fds(int *fd_in, int *fd_out);

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

// utils/utils_string_array.c
char	**copy_string_array(char **strings);
int		ft_strlen_array(char **array);

// filenavs.c
char	*cd(char *path, char *new_path);
void	pwd(char **path_dirs);
void	minishell_exit(void);

// signals/signal.c
void	ctrl_c(int sig);

#endif