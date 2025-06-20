/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:13:01 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/20 08:58:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>

// 0644: user can read/write, others can read. reasonable/safe setting.
# define PIPEX_CREATE_PERMISSIONS 0644
// 1048576: 1MB buffer size for heredoc. ARGMAX on CODAM (2025) is 2MB.
# define PIPEX_HEREDOC_INITIAL_BUFFER_SIZE 1048576

# define QUOTES "\'\""

typedef struct s_pipex
{
	int		argc;
	char	**argv;
	char	**envp;
	char	***cmd_arg;
	bool	*cmd_not_found;
	char	**cmd_path;
	char	**path_dirs;
	size_t	cmd_offset;
	size_t	cmd_total;
	int		fdin;
	int		fdout;
	int		outfile_flags;
	bool	hdoc;
	char	*infile;
	char	*outfile;
	int		exit_code;
}	t_pipex;

typedef struct s_heredoc_pipex
{
	char	*line;
	size_t	buffer_size;
	size_t	len;
	ssize_t	bytes_read;
	char	c;
}	t_heredoc_pipex;

/* next_string_quote parsing struct
str_len		->	Length of the string
str_pos		->	Starting string position
end_pos		->	End position of string
empty_quotes->	Identifies empty quote pairs
write_pos	->	Position of substring when copying from string
*/
typedef struct s_nxtsq
{
	size_t	str_len;
	size_t	str_pos;
	size_t	end_pos;
	bool	empty_quotes;
	size_t	write_pos;
}	t_nxtsq;

// pipex_main.c
int		run_pipex_once(int argc, char **argv, char **envp);

// pipex_interactive.c
int		run_pipex_interactive(const char *argv_0, char **envp);

bool	get_args_from_line(char *line, const char *arg_0, \
			int *argc, char ***argv);

// ft_mem_utils.c

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t nmemb, size_t size);

void	*ft_memcpy(void *dst, const void *src, size_t n);

// ft_str_utils.c

char	*ft_strdup(char const *src);

char	*ft_strjoin(char const *s1, char const *s2);

size_t	ft_strlen(const char *str);

int		ft_strncmp(const char *s1, const char *s2, size_t n);

// utils_error.c

void	put_stderr(const char *error);

void	put_stderr_2(const char *str1, const char *str2);

void	put_stderr_3(const char *str1, const char *str2, const char *str3);

// utils_exit.c

void	close_fds_exit_error_free(t_pipex *px, const char *error, int *pipefd);

void	exit_error(const char *error);

void	exit_error_free(t_pipex *px, const char *error);

void	exit_free_with_code(t_pipex *px, int exit_code);

// utils_free.c

void	free_everything(t_pipex *px);

void	safe_free(char **ptr);

void	safe_free_2d(char ***ptr);

void	safe_free_3d(char ****ptr);

void	safe_free_bool(bool **ptr);

// utils_mem.c

void	*realloc_with_oldsize(void *ptr, size_t old_size, size_t new_size);

// utils_path.c

char	*get_path_from_env(char **envp);

char	*get_valid_cmd_full_path(char **path_dirs, char *cmd);

char	*make_cmd_full_path(const char *dir, const char *cmd);

// utils_split_quotes.c

char	**split_charset_using_quote(char *str, char *charset);

// utils_split_single_delimiter.c

char	**split_single_delimiter(char const *s, const char c);

// utils_string.c

int		c_in_str(char c_to_find, char *str);

char	*get_empty_string(void);

// pipex_heredoc.c

void	heredoc_pipe(t_pipex *px);

void	read_one_heredoc_line(t_heredoc_pipex *hdoc);

void	init_heredoc(t_heredoc_pipex *hdoc);

void	free_heredoc_line(t_heredoc_pipex *hdoc);

// pipex_initialize.c

void	initialize_pipex(t_pipex *px, int argc, char **argv, char **envp);

void	initialize_pipex_commands(t_pipex *px);

void	open_infile(t_pipex *px);

// pipex_process.c

int		execute_command(t_pipex *px, size_t cmd_i);

void	parse_command(t_pipex *px, size_t cmd_i);

void	process_piped_command(t_pipex *px, size_t cmd_i);

pid_t	process_last_command(t_pipex *px, size_t cmd_i);

#endif
