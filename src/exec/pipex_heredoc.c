/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:53:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/13 00:14:47 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_heredoc(t_heredoc_pipex *hdoc)
{
	hdoc->buffer_size = PIPEX_HEREDOC_INITIAL_BUFFER_SIZE;
	hdoc->line = ft_calloc(hdoc->buffer_size + 1, sizeof(char));
	if (hdoc->line == NULL)
		exit_error("memory allocation failed for heredoc line");
	hdoc->len = 0;
	hdoc->bytes_read = 0;
}

void	free_heredoc_line(t_heredoc_pipex *hdoc)
{
	safe_free((void *)hdoc->line);
	hdoc->line = NULL;
}

void	start_heredoc_loop(t_heredoc_pipex *hdoc, const char *delimiter)
{
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		read_one_heredoc_line(hdoc);
		if (hdoc->len == 0 || hdoc->bytes_read < 0)
			break ;
		if (hdoc->len > 0 && hdoc->line[hdoc->len - 1] == '\n')
		{
			hdoc->line[hdoc->len - 1] = '\0';
			if (ft_strlen(hdoc->line) == delimiter_len
				&& ft_strncmp(delimiter, hdoc->line, delimiter_len) == 0)
				break ;
			hdoc->line[hdoc->len - 1] = '\n';
		}
		else if (ft_strlen(hdoc->line) == delimiter_len
			&& ft_strncmp(delimiter, hdoc->line, delimiter_len) == 0)
			break ;
		write(STDOUT_FILENO, hdoc->line, ft_strlen(hdoc->line));
		if (hdoc->bytes_read == 0 || hdoc->line[hdoc->len - 1] != '\n')
			break ;
	}
	free_heredoc_line(hdoc);
}

void	read_one_heredoc_line(t_heredoc_pipex *hdoc)
{
	hdoc->len = 0;
	write(STDERR_FILENO, "heredoc> ", 9);
	while (1)
	{
		hdoc->bytes_read = read(STDIN_FILENO, &hdoc->c, 1);
		if (hdoc->bytes_read <= 0)
			break ;
		if (hdoc->len + 1 >= hdoc->buffer_size)
		{
			hdoc->line = realloc_with_oldsize(
					hdoc->line, hdoc->buffer_size + 1,
					(hdoc->buffer_size * 2) + 1);
			if (!hdoc->line)
			{
				free_heredoc_line(hdoc);
				exit_error("memory allocation failed for command arguments");
			}
			hdoc->buffer_size *= 2;
		}
		hdoc->line[hdoc->len++] = hdoc->c;
		if (hdoc->c == '\n')
			break ;
	}
	hdoc->line[hdoc->len] = '\0';
}

void	heredoc_pipe(t_pipex *px)
{
	t_heredoc_pipex	hdoc;
	int				pipefd[2];
	pid_t			pid;

	if (pipe(pipefd) == -1)
		exit_error_free(px, "pipe");
	pid = fork();
	if (pid == -1)
		close_fds_exit_error_free(px, "fork", pipefd);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			close_fds_exit_error_free(px,
				"error: failed to redirect output", pipefd);
		close(pipefd[1]);
		init_heredoc(&hdoc);
		start_heredoc_loop(&hdoc, px->argv[2]);
		exit(0);
	}
	close(pipefd[1]);
	px->fdin = pipefd[0];
	waitpid(pid, NULL, 0);
}
