/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_base.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 11:35:14 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 12:43:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BASE_H
# define MINISHELL_BASE_H

# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdarg.h>
# include "libft.h"
# include "minishell_errors.h"

/* ========================================================================== */
/*                          BASIC DATA STRUCTURES                             */
/* ========================================================================== */

typedef struct s_readbuf
{
	char	buf[4096];
	ssize_t	len;
	ssize_t	pos;
}	t_readbuf;

/*
	Struct to hold per-call state of the readline_noninteractive.
	- This tracks the accumulating line buffer made and its current length
	during a single call.
	- It is initialized fresh each time and freed/returned when the
	line is emitted. Unlike t_readbuf, this does not persist across calls.
*/
typedef struct s_rln_state
{
	char	*line_made;
	size_t	made_len;
	ssize_t	end;
}	t_rln_state;

/* ========================================================================== */
/*                      UTILITY FUNCTIONS (NO STRUCT DEPS)                    */
/* ========================================================================== */

/* utils/utils_error.c */
void	msg_err(const char *error);
void	msg_err_2(const char *str1, const char *str2);
void	msg_err_3(const char *str1, const char *str2, const char *str3);

/* utils/utils_error_2.c */
void	msg_perr(const char *error);

/* utils/utils_free.c */
bool	make_string_free(char **string);
void	safe_free(void **ptr);
void	safe_free_2d_string(char ***ptr);
void	safe_free_str(char **ptr);

/* utils/utils_math.c */
int		max_int(int a, int b);
int		min_int(int a, int b);

/* utils/utils_string.c */
int		add_line_to_string(char **string, char **line);
char	*get_empty_string(void);
bool	set_empty_string(char **to_empty);

/* utils/utils_string_array.c */
char	**copy_string_array(char **strings);
int		ft_strlen_array(char **array);

/* utils/utils_malloc.c */
int		xe_calloc(void **ptr, int *err, size_t nmemb, size_t size);
int		xe_malloc(void **ptr, int *err, size_t nmemb, size_t size);

/* utils/utils_malloc_simple.c */
int		x_calloc_char(char **ptr, size_t count);
int		x_calloc_charptr(char ***ptr, size_t count);
int		x_calloc_int(int **ptr, size_t count);

/* utils/utils_malloc_types.c */
int		xe_calloc_char(char **ptr, int *err, size_t count);
int		xe_calloc_charptr(char ***ptr, int *err, size_t count);
int		xe_calloc_int(int **ptr, int *err, size_t count);

/* utils/utils_path.c */
char	*get_path_from_env(char **envp);
char	*get_valid_cmd_full_path(char **path_dirs, char *cmd);
char	*make_cmd_full_path(const char *dir, const char *cmd);
int		update_path_dirs(char ***path_dirs, char **envp);

/* ========================================================================== */
/*                            SHORTCUT FUNCTIONS                              */
/* ========================================================================== */

/* utils/shortcuts/utils_r_err_msg.c */
void	*r_free_str_null(char **to_free);
void	*r_msg_err_null(const char *error);
int		r_msg_err_free_str(const char *error, char **to_free, int ret);

/* utils/shortcuts/utils_r_plus.c */
void	*r_free_null(void **ptr);
int		r_free_str(char **to_free, int ret);
int		r_free_two_str(char **str_a, char **str_b, int ret);

/* utils/shortcuts/utils_r_malloc.c */
int		r_msg_err(const char *error_msg, int ret);
int		r_msg_two_err(const char *str1, const char *str2, int ret);
int		r_msg_perr(const char *error_msg, int ret);
void	*r_msg_perr_null(const char *error);

#endif
