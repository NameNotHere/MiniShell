/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_assist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 00:00:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/04 16:59:36 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	hdoc_process_line(t_msh *sh, t_redir *redir,\
	char **hdoc_line, char **hdoc_string)
{
	if (!*hdoc_line)
		return (safe_free_str(hdoc_line), true);
	if (!redir->quoted && !expand_string_variables(sh, hdoc_line, true))
		return (safe_free_str(hdoc_line), false);
	if (ft_strcmp(redir->string, *hdoc_line) == 0)
		return (safe_free_str(hdoc_line), false);
	if (add_line_to_string(hdoc_string, hdoc_line) == EXIT_FAILURE)
		return (false);
	return (true);
}

static char	*hdoc_handle_input(t_msh *sh, t_redir *redir)
{
	char	*hdoc_line;

	g_sig = 0;
	hdoc_line = get_shell_line(sh, HDOC_PROMPT);
	if (g_sig == SIGINT)
		return (safe_free_str(&hdoc_line), NULL);
	if (!hdoc_line)
	{
		msg_err_3(
			E_HDOC_EOF_START,
			redir->string, E_HDOC_EOF_END);
		return (NULL);
	}
	return (hdoc_line);
}

char	*hdoc_loop(t_msh *sh, t_redir *redir)
{
	char	*hdoc_string;
	char	*hdoc_line;

	hdoc_string = NULL;
	sh->exit_code = EXIT_SUCCESS;
	if (!set_heredoc_sig())
		return (msg_err(E_HEREDOC_SIG), NULL);
	while (true)
	{
		hdoc_line = hdoc_handle_input(sh, redir);
		if (!hdoc_line)
		{
			if (g_sig == SIGINT)
			{
				safe_free_str(&hdoc_string);
				sh->exit_code = 130;
				return (NULL);
			}
			return (hdoc_string);
		}
		if (!hdoc_process_line(sh, redir, &hdoc_line, &hdoc_string))
			break ;
	}
	return (hdoc_string);
}

void	hdoc_err(t_msh *sh, int *write_fd, int *redir_fd, char *hdoc_str)
{
	safe_close_2_fds(write_fd, redir_fd);
	safe_free_str(&hdoc_str);
	sh->exit_code = EXIT_FAILURE;
	msg_err(E_HEREDOC_REDIR);
	return ;
}
