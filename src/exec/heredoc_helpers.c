/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:25:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/11 12:42:29 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "minishell.h"

static int	setup_hdoc_fd(int tmp_fd, char *hdoc_str, t_redir *redir)
{
	char	*fd_path;
	int		read_fd;

	fd_path = build_fd_path(tmp_fd);
	if (!fd_path)
		return (hdoc_err(&tmp_fd, hdoc_str, E_ALLOC_HEREDOC));
	read_fd = open(fd_path, O_RDONLY);
	free(fd_path);
	safe_close_fd(&tmp_fd);
	if (read_fd == -1)
		return (hdoc_err(NULL, hdoc_str, E_DUP_HEREDOC));
	redir->fd = read_fd;
	return (EXIT_SUCCESS);
}

static int	hdocs_cleanup_sigint(t_redir *redir, int *tmp_fd, int *dup_fd)
{
	safe_close_2_fds(tmp_fd, dup_fd);
	if (!redir)
		return (EXIT_SIGINT);
	safe_close_fd(&redir->fd);
	redir = redir->next;
	while (redir)
	{
		if (redir->ty == REDIR_HEREDOC)
			safe_close_fd(&redir->fd);
		redir = redir->next;
	}
	return (EXIT_SIGINT);
}

int	hdoc_redir(t_msh *sh, t_redir *redir)
{
	char	*hdoc_str;
	int		tmp_fd;
	int		ret;

	if (!redir)
		return (EXIT_SUCCESS);
	if (redir && redir->ty != REDIR_HEREDOC)
		return (hdoc_redir(sh, redir->next));
	tmp_fd = open("/tmp", O_TMPFILE | O_RDWR, 0600);
	if (tmp_fd == -1)
		return (hdoc_err(NULL, NULL, E_OPEN_HEREDOC));
	hdoc_str = hdoc_loop(sh, redir);
	if (hdoc_str == NULL && g_sig == SIGINT)
		return (hdocs_cleanup_sigint(redir->next, &tmp_fd, NULL));
	if (hdoc_str == NULL && set_empty_string(&hdoc_str) == false)
		return (hdoc_err(&tmp_fd, hdoc_str, E_ALLOC_HEREDOC));
	if ((write(tmp_fd, hdoc_str, ft_strlen(hdoc_str)) == -1)
		|| (ft_strlen(hdoc_str) > 0 && write(tmp_fd, "\n", 1) == -1))
		return (hdoc_err(&tmp_fd, hdoc_str, E_WRITE_HEREDOC));
	ret = setup_hdoc_fd(tmp_fd, hdoc_str, redir);
	safe_free_str(&hdoc_str);
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (hdoc_redir(sh, redir->next));
}
