/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:28:55 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 19:48:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Closes open fds when they are not STDIN, STDOUT or STDERR
*/
void	safe_close_fd(int *fd)
{
	int	fd_value;

	if (!fd || *fd <= STDERR_FILENO)
		return ;
	fd_value = *fd;
	*fd = -1;
	close(fd_value);
}

void	safe_close_2_fds(int *fd_one, int *fd_two)
{
	safe_close_fd(fd_one);
	safe_close_fd(fd_two);
}

int	cleanup_all_fds(t_msh *sh, int pipefd[2], int *fd_in, int *fd_out)
{
	safe_close_2_fds(&pipefd[0], &pipefd[1]);
	safe_close_2_fds(fd_in, fd_out);
	return (sh->exit_code);
}

/*
	Saves the current stdin/stdout file descriptors.
	Returns true on success, false on failure.
*/
bool	save_std_fds(int *saved_fd_stdin, int *saved_fd_stdout)
{
	*saved_fd_stdin = dup(STDIN_FILENO);
	*saved_fd_stdout = dup(STDOUT_FILENO);
	if (*saved_fd_stdin == -1 || *saved_fd_stdout == -1)
	{
		if (*saved_fd_stdin != -1)
			close(*saved_fd_stdin);
		if (*saved_fd_stdout != -1)
			close(*saved_fd_stdout);
		return (false);
	}
	return (true);
}

/*
	Restores previously saved stdin/stdout file descriptors.
*/
void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
