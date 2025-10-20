/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_assist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 00:00:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/11 00:00:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	hdoc_process_line(t_msh *sh, t_redir *redir,\
	char **hdoc_line, char **hdoc_string)
{
	if (!*hdoc_line)
		return (safe_free_string(hdoc_line), true);
	if (!redir->quoted && !expand_string_variables(sh, hdoc_line))
		return (safe_free_string(hdoc_line), false);
	if (ft_strcmp(redir->string, *hdoc_line) == 0)
		return (safe_free_string(hdoc_line), false);
	if (add_line_to_string(hdoc_string, hdoc_line) == EXIT_FAILURE)
		return (false);
	return (true);
}

static char	*hdoc_handle_input(t_msh *sh)
{
	char	*hdoc_line;

	g_sig = 0;
	hdoc_line = get_shell_line(sh, HDOC_PROMPT);
	if (g_sig == SIGINT)
		return (safe_free_string(&hdoc_line), NULL);
	if (!hdoc_line)
	{
		msg_err("warning: here-document delimited by end-of-file");
		return (NULL);
	}
	return (hdoc_line);
}

char	*hdoc_loop(t_msh *sh, t_redir *redir)
{
	char	*hdoc_string;
	char	*hdoc_line;

	hdoc_string = NULL;
	sh->exit_code = EXIT_FAILURE;
	if (!set_heredoc_sig())
		return (msg_err("failed to set heredoc signal handler"), NULL);
	while (true)
	{
		hdoc_line = hdoc_handle_input(sh);
		if (!hdoc_line)
			return (hdoc_string);
		if (!hdoc_process_line(sh, redir, &hdoc_line, &hdoc_string))
			break ;
	}
	sh->exit_code = EXIT_SUCCESS;
	return (hdoc_string);
}

void	hdoc_err(t_msh *sh, int *write_fd, int *redir_fd, char *hdoc_str)
{
	safe_close_2_fds(write_fd, redir_fd);
	safe_free_string(&hdoc_str);
	sh->exit_code = EXIT_FAILURE;
	msg_err("heredoc redir failed");
	return ;
}
