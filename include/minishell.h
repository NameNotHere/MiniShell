/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 15:40:47 by tda-roch         ###   ########.fr       */
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
# include <termios.h>

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

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_PERM_DENIED 126
# define EXIT_SIGINT 130

// 0644: user can read/write, others can read. reasonable/safe setting.
# define OUTPUT_PERMISSIONS 0644

typedef enum e_flow
{
	EXEC_FLOW,
	CONTINUE_FLOW,
	BREAK_FLOW
}	t_flow;

// minishell_initialize.c
int		initialize_minishell(t_msh *sh, int argc, char **argv, char **envp);

// int		minishell_mainloop(t_msh *sh);

// builtins_cd_pwd_env.c
int		execute_builtin(t_msh *sh, t_cmd *cmd);
int		x_cd(t_msh *sh, t_cmd *cmd);
int		x_pwd(t_msh *sh, t_cmd *cmd);
int		x_env(t_msh *sh, int argc);
int		x_unset(t_msh *sh, t_cmd cmd);
int		x_echo(char **argv, int argc);

// builtins_exit_export.c
int		x_exit(t_msh *sh, t_cmd cmd);
int		x_export(t_msh *sh, t_cmd cmd);
int		handle_export_assignment(t_msh *sh, char *name, char *equals_pos);

// exec/execute.c
int		exec_ast(t_msh *sh, t_ast *node, int fd_in, int fd_out);
int		exec_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out);

// exec/execute_cleanup.c
void	free_everything(t_msh *sh);
void	shell_line_cleanup(t_msh *sh);

// exec/execute_cmd.c
int		execute_command(t_msh *sh, t_cmd *cmd);
void	exec_left(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);
void	exec_right(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);

// exec/execute_cmd_single.c
int		exec_single_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out);
void	exec_single_cmd_in_child(t_msh *sh, int fd_in, int fd_out, t_cmd *cmd);
int		exec_single_builtin(t_msh *sh, t_cmd *cmd);

// exec/execute_cmd_redir.c
bool	execute_redirection(t_msh *sh, t_redir *redir);

// exec/execute_cmd_redir_open.c
int		open_input_redirection(t_msh *sh, char *filename);
int		open_output_redirection(t_msh *sh, char *filename);
int		open_append_redirection(t_msh *sh, char *filename);

// exec/heredoc.c
int		heredoc_ast_node(t_msh *sh, t_ast *node);
int		heredoc_pipe_node(t_msh *sh, t_pipe *pipe_node);
int		hdoc_redir(t_msh *sh, t_redir *redir);

// exec/heredoc_assist.c
char	*hdoc_loop(t_msh *sh, t_redir *redir);
int		hdoc_err(int *tmp_fd, char *hdoc_str, char *error_msg);

// exec/lookup_cmd_fullpath.c
int		lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// exec/safe_pipe.c
bool	safe_pipe(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);

// exec/safe_fork.c
pid_t	safe_fork_cmd(t_msh *sh, int *fd_in, int *fd_out);
pid_t	safe_fork_pipe(t_msh *sh, int *pipe_fds, int *fd_in, int *fd_out);

// parse/parse_line.c
int		parse_line(t_msh *sh, t_ast *ast, char *string);

// utils/utils_dup2.c
void	try_dup2_stdout(t_msh *sh, int *fd_out);
void	try_dup2_stdin(t_msh *sh, int *fd_in);
void	try_dup2(t_msh *sh, int *fd_in, int *fd_out);

// utils/utils_env.c
bool	is_var_in_env(t_msh *sh, char *var, int *envp_idx);
char	*get_env_value_by_idx(t_msh *sh, char *var_name, int envp_idx);
char	*get_env_value_by_name(t_msh *sh, char *var_name);
int		update_shell_level_var(t_msh *sh);
int		initialize_null_env(t_msh *sh);

// utils/utils/exit.c
void	close_fds_exit_error_free(t_msh *sh, const char *error, int *fd_in,
			int *fd_out);
void	exit_error_free(t_msh *sh, const char *error);
void	exit_free_with_code(t_msh *sh, int exit_code);
int		handle_execute_command_errors(t_cmd *cmd);

// utils/utils_fd.c
void	safe_close_fd(int *fd);
void	safe_close_2_fds(int *fd_one, int *fd_two);
void	safe_close_4_fds(int *fd_one, int *fd_two, int *fd_three, int *fd_four);
int		cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);
bool	save_std_fds(int *saved_stdin, int *saved_stdout);
void	restore_std_fds(int saved_stdin, int saved_stdout);

// utils/utils_readine
bool	readline_noninteract(int fd, t_readbuf *st, char **out);

// utils/utils_readline_state.c
bool	add_chunk(t_rln_state *st, const char *src, size_t n);
bool	rln_flush_line(t_rln_state *st, char **line);
bool	rln_init(t_rln_state *st, t_readbuf *rb, char **line);
bool	rln_emit_line(t_rln_state *st, t_readbuf *rb, char **line);

// utils/utils_set_exit_code.c
void	set_exit_code(t_msh *sh, int exit_code);
void	set_exit_msg(t_msh *sh, int exit_code, const char *error_msg);
void	set_exit_perr(t_msh *sh, const char *error_msg);
void	*set_exit_perr_null(t_msh *sh, const char *error_msg);

// utils/utils_r_set_exit.c
int		r_set_exit(t_msh *sh, int exit_code);
int		r_set_exit_msg(t_msh *sh, int exit_code, const char *error_msg);
int		r_set_exit_perr(t_msh *sh, const char *error_msg);
int		r_set_exit_ret(t_msh *sh, int exit_code, int ret);
int		r_msg_err(const char *error_msg, int ret);
int		r_msg_perr(const char *error_msg, int ret);
int		r_free_everything(t_msh *sh, int ret);

// utils/utils_r_plus.c
int		r_free_str(char **to_free, int ret);
int		r_free_two_str(char **str_a, char **str_b, int ret);
int		r_free_str_perr(t_msh *sh, char **to_free, const char *error_msg);

// utils/utils_r_err_msg.c
int		r_msg_err_free_str(const char *error, char **to_free, int ret);
void	*msg_err_null(const char *error);
void	*msg_perr_null(const char *error);

// utils/utils_string.c
char	*get_shell_line(t_msh *sh, char *prompt);
int		add_line_to_string(char **string, char **line);
char	*get_empty_string(void);
bool	set_empty_string(char **to_empty);

// utils/utils_string_array.c
char	**copy_string_array(char **strings);
int		ft_strlen_array(char **array);

// utils/utils_token.c
bool	is_valid_cmd_token(t_token_ty token_type);

//envp assistance
int		search_name(char *name, char **envp);
int		length_till_equal(char *str);
int		change_env_val_idx(char *name, char *new_value, int index,
			char ***envp);
int		change_env_val(char *name, char *new_value, char ***envp);

int		add_env_var(char ***envp, char *name, char *value);
int		envp_len(char **envp);

// utils/ft_strndup
char	*ft_strndup(const char *src, int size);

// utils/ft_strcmp
int		ft_strcmp(const char *s1, const char *s2);

//is_builtin.c
bool	is_builtin(char *str);
int		x_echo(char **argv, int argc);

// utils/ft_realloc
void	*ft_realloc(void *ptr, size_t new_size, size_t old_size);

// utils/unclosed_quotes.c
int		unclosed_quotes(const char *line);
bool	error_unclosed_quotes(const char *line);

// utils/utils_fd_path.c
char	*build_fd_path(int fd);

// parser/lex_helpers.c
void	skip_spaces(int *i, char *str);
bool	is_token_char(char *str, int pos);
void	update_quoted_len(char *str, int *len, int i);
void	calculate_token_word_len(char *str, int *i, int *len);

// parser/line_var_expand_helper_pro.c
bool	advanced_substitutions(t_var_expand *ve, char **str_ptr);

#endif
