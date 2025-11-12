/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 12:51:45 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "minishell_parser.h"
# include "minishell_signal.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
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

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_PERM_DENIED 126
# define EXIT_SIGINT 130

// 0644: user can read/write, others can read. reasonable/safe setting.
# define OUTPUT_PERMISSIONS 0644

/* enum for flow control (returned value to break or continue from loop)*/
typedef enum e_flow
{
	EXEC_FLOW,
	CONTINUE_FLOW,
	BREAK_FLOW
}	t_flow;

// init/
int		initialize_minishell(t_msh *sh, int argc, char **argv, char **envp);
char	*get_shell_line(t_msh *sh, char *prompt);

// exec: builtins
int		execute_builtin(t_msh *sh, t_cmd *cmd);
int		x_cd(t_msh *sh, t_cmd *cmd);
int		x_pwd(t_msh *sh, t_cmd *cmd);
int		x_env(t_msh *sh, int argc);
int		x_unset(t_msh *sh, t_cmd cmd);
int		x_echo(char **argv, int argc);
int		x_exit(t_msh *sh, t_cmd cmd);
int		x_export(t_msh *sh, t_cmd cmd);
int		handle_export_assignment(t_msh *sh, char *name, char *equals_pos);

// exec: cleanup
void	free_everything(t_msh *sh);
void	shell_line_cleanup(t_msh *sh);

// exec: ast execution
int		exec_ast(t_msh *sh, t_ast *node, int fd_in, int fd_out);
int		exec_ast_root(t_msh *sh, t_ast *node, int fd_in, int fd_out);

// exec: cmd
int		execute_command(t_msh *sh, t_cmd *cmd);
void	exec_left(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);
void	exec_right(t_msh *sh, t_ast *node, int pipefd[2], int *fd_in_out[2]);
int		exec_single_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out);
void	exec_single_cmd_in_child(t_msh *sh, int fd_in, int fd_out, t_cmd *cmd);
int		exec_single_builtin(t_msh *sh, t_cmd *cmd);
bool	execute_redirection(t_msh *sh, t_redir *redir);
int		open_input_redirection(t_msh *sh, char *filename);
int		open_output_redirection(t_msh *sh, char *filename);
int		open_append_redirection(t_msh *sh, char *filename);

// exec: heredoc
int		heredoc_ast_node(t_msh *sh, t_ast *node);
int		heredoc_pipe_node(t_msh *sh, t_pipe *pipe_node);
int		hdoc_redir(t_msh *sh, t_redir *redir);
char	*hdoc_loop(t_msh *sh, t_redir *redir);
int		hdoc_err(int *tmp_fd, char *hdoc_str, char *error_msg);

// exec: lookup command fullpath
int		lookup_all_cmd_fullpaths(t_msh *sh, t_ast *node);

// exec: syscall wrappers
bool	safe_pipe(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);
pid_t	safe_fork_cmd(t_msh *sh, int *fd_in, int *fd_out);
pid_t	safe_fork_pipe(t_msh *sh, int *pipe_fds, int *fd_in, int *fd_out);

// parse: line
int		parse_line(t_msh *sh, t_ast *ast, char *string);

// envp assistance
int		search_name(char *name, char **envp);
int		length_till_equal(char *str);
int		change_env_val_idx(char *name, char *new_value, int index,
			char ***envp);
int		change_env_val(char *name, char *new_value, char ***envp);
int		add_env_var(char ***envp, char *name, char *value);
int		envp_len(char **envp);

/* ========================================================================== */
/*                              UTILS FUNCTIONS                               */
/* ========================================================================== */

// dup2
void	try_dup2_stdout(t_msh *sh, int *fd_out);
void	try_dup2_stdin(t_msh *sh, int *fd_in);
void	try_dup2(t_msh *sh, int *fd_in, int *fd_out);

// env
bool	is_var_in_env(t_msh *sh, char *var, int *envp_idx);
char	*get_env_value_by_idx(t_msh *sh, char *var_name, int envp_idx);
char	*get_env_value_by_name(t_msh *sh, char *var_name);
int		update_shell_level_var(t_msh *sh);
int		initialize_null_env(t_msh *sh);

// exit
void	close_fds_exit_error_free(t_msh *sh, const char *error, int *fd_in,
			int *fd_out);
void	exit_error_free(t_msh *sh, const char *error);
void	exit_free_with_code(t_msh *sh, int exit_code);
int		handle_execute_command_errors(t_cmd *cmd);

// fd
void	safe_close_fd(int *fd);
void	safe_close_2_fds(int *fd_one, int *fd_two);
int		cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out);
bool	save_std_fds(int *saved_stdin, int *saved_stdout);
void	restore_std_fds(int saved_stdin, int saved_stdout);
char	*build_fd_path(int fd);

// readline
bool	readline_noninteract(int fd, t_readbuf *st, char **out);
bool	add_chunk(t_rln_state *st, const char *src, size_t n);
bool	rln_flush_line(t_rln_state *st, char **line);
bool	rln_init(t_rln_state *st, t_readbuf *rb, char **line);
bool	rln_emit_line(t_rln_state *st, t_readbuf *rb, char **line);

#endif
