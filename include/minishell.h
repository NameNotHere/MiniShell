/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 18:50:30 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_parser.h"
# include "minishell_signal.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <unistd.h>
/*
	\033[96m = cyan
	colors need to be wrapped in \001 and \002
		for readline to calculate prompt lenght correctly
	prompt is: star+arrow(cyan)
	user input has default term color
*/
# define MSH_PROMPT "\001\033[96m\002✶➜\001\033[0m\002 "
# define HDOC_PROMPT "hdoc > "

// alternative simpler prompts commented out below:
// # define MSH_PROMPT "$ "
// # define HDOC_PROMPT "> "

// 0644: user can read/write, others can read. reasonable/safe setting.
# define OUTPUT_PERMISSIONS 0644

typedef enum e_err_code
{
	E_INVALID_REDIR = 200,
	E_MULTIPLE_CMD
}	t_err_code;

# define E_INVALID_REDIR_MSG "syntax error: invalid redirection, missing string"
# define E_MULTIPLE_MSG "syntax error: multiple commands"

typedef enum e_flow
{
	EXEC_FLOW,
	CONTINUE_FLOW,
	BREAK_FLOW
}	t_flow;

// minishell_main.c
int		initialize_minishell(t_msh *sh, int argc, char **argv, char **envp);
int		minishell_mainloop(t_msh *sh);

// builtins.c
int		execute_builtin(t_msh **sh, t_cmd *cmd);

// builtins_echo.c
int		ft_echo(char **argv, int argc);

// builtins_exit_export.c
int		ft_exit(t_msh *sh, t_cmd cmd);
int		ft_export(t_msh **sh, t_cmd cmd);

// exec/execute.c
int		exec_ast(t_msh *sh, t_ast *node, int fd_in, int fd_out);
int		exec_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out);

// exec/execute_cleanup.c
void	free_everything(t_msh *sh);
void	shell_line_cleanup(t_msh *sh);

// exec/execute_cmd.c
int		execute_command(t_msh *sh, t_cmd *cmd);
int		exec_single_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out);
void	exec_single_cmd_in_child(t_msh *sh, int fd_in, int fd_out, t_cmd *cmd);
void	exec_left(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);
void	exec_right(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);

// exec/execute_cmd_redir.c
void	execute_redirection(t_msh *sh, t_redir *redir);

// exec/execute_cmd_redir_open.c
int		open_input_redirection(t_msh *sh, char *filename);
int		open_output_redirection(t_msh *sh, char *filename);
int		open_append_redirection(t_msh *sh, char *filename);

// exec/heredoc.c
int		heredoc_ast_node(t_msh *sh, t_ast *node);

// exec/lookup_cmd_fullpath.c
int		lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// exec/safe_pipe.c
int		safe_pipe(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);

// exec/safe_fork.c
pid_t	safe_fork_cmd(t_msh *sh, int *fd_in, int *fd_out);
pid_t	safe_fork_pipe(t_msh *sh, int *pipe_fds, int *fd_in, int *fd_out);

// parse/parse_line.c
int		parse_line(t_msh *sh, t_ast *ast, char *string);

// utils/utils_char.c
int		ft_valid_var_char(int c);
int		ft_is_singlequote(int c);
int		ft_is_doublequote(int c);
int		ft_is_quote(int c);

// utils/utils_dup2.c
void	try_dup2_stdout(t_msh *sh, int *fd_out);
void	try_dup2_stdin(t_msh *sh, int *fd_in);
void	try_dup2(t_msh *sh, int *fd_in, int *fd_out);

// utils/utils_env.c
bool	is_var_in_env(t_msh *sh, char *var, int *envp_index);
char	*get_env_value(t_msh *sh, char *var_name, int envp_index);

// utils/utils_error.c
void	put_stderr(const char *error);
void	put_stderr_2(const char *str1, const char *str2);
void	put_stderr_3(const char *str1, const char *str2, const char *str3);

// utils/utils_error_shell.c
void	put_stderr_code(t_msh *sh, const char *error, int exit_code);

// utils/utils/exit.c
void	close_fds_exit_error_free(t_msh *sh, const char *error, int *fd_in,
			int *fd_out);
void	exit_error_free(t_msh *sh, const char *error);
void	exit_free_with_code(t_msh *sh, int exit_code);
int		handle_execute_command_errors(t_msh *sh, t_cmd *cmd);

// utils/utils_fd.c
void	safe_close_fd(int *fd);
void	safe_close_2_fds(int *fd_one, int *fd_two);
void	safe_close_4_fds(int *fd_one, int *fd_two, int *fd_three, int *fd_four);
int		cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);

// utils/utils_free.c
void	safe_free(void **ptr);
void	safe_free_string(char **ptr);
void	safe_free_2d_string(char ***ptr);

// utils/utils_path.c
char	*make_cmd_full_path(const char *dir, const char *cmd);
char	*get_valid_cmd_full_path(char **path_dirs, char *cmd);
char	*get_path_from_env(char **envp);

// utils/utils_readine
bool	readline_on_tty(const char *prompt, char **line);
bool	readline_noninteract(int fd, t_readbuf *st, char **out);

// utils/utils_readline_state.c
bool	add_chunk(t_rln_state *st, const char *src, size_t n);
bool	rln_flush_line(t_rln_state *st, char **line);
bool	rln_init(t_rln_state *st, t_readbuf *rb, char **line);
bool	rln_emit_line(t_rln_state *st, t_readbuf *rb, char **line);

// utils/utils_string.c
char	*get_shell_line(t_msh *sh, char *prompt);
int		add_line_to_string(char **string, char **line);
char	*get_empty_string(void);
bool	set_empty_string(char **to_empty);
bool	make_string_free(char **string);

// utils/utils_string_array.c
char	**copy_string_array(char **strings);
int		ft_strlen_array(char **array);

// utils/utils_token.c
bool	is_valid_cmd_token(t_token_ty token_type);

// filenavs.c
char	*cd(char *path, char *new_path);
void	pwd(char **path_dirs);
void	minishell_exit(void);

//envp assistance
int		search_name(char *name, char **envp);
int		length_till_equal(char *str);
int		change_env_value(char *name, char *new_value, char ***envp);
int		add_env_var(char ***envp, char *name, char *value);
int		envp_len(char **envp);

// utils/ft_strndup
char	*ft_strndup(const char *src, int size);

// utils/ft_strcmp
int		ft_strcmp(const char *s1, const char *s2);

//is_builtin.c
int		is_builtin(char *str);
int		ft_echo(char **argv, int argc);
int		ft_export(t_msh **sh, t_cmd cmd);

// utils/ft_realloc
void	*ft_realloc(void *ptr, size_t new_size, size_t old_size);

// utils/unclosed_quotes.c
int		unclosed_quotes(const char *line);

#endif