/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_assist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 00:00:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/12 12:43:21 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hdoc_add_line(t_msh *sh, t_redir *redir,\
	char **hdoc_line, char **hdoc_string)
{
	if (!*hdoc_line)
		return (r_free_str(hdoc_line, EXIT_SUCCESS));
	if (!redir->quoted && !expand_string_variables(sh, hdoc_line, true))
		return (r_free_str(hdoc_line, EXIT_FAILURE));
	if (ft_strcmp(redir->string, *hdoc_line) == 0)
		return (r_free_str(hdoc_line, EXIT_FAILURE));
	if (add_line_to_string(hdoc_string, hdoc_line) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static char	*hdoc_handle_input(t_msh *sh, t_redir *redir)
{
	char	*hdoc_line;

	g_sig = 0;
	hdoc_line = get_shell_line(sh, HDOC_PROMPT);
	if (g_sig == SIGINT)
		return (r_free_str_null(&hdoc_line));
	if (!hdoc_line)
	{
		msg_err_3(E_HDOC_EOF_START, redir->string, E_HDOC_EOF_END);
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
		return (r_msg_err_null(E_HEREDOC_SIG));
	while (true)
	{
		hdoc_line = hdoc_handle_input(sh, redir);
		if (!hdoc_line)
		{
			if (g_sig == SIGINT)
			{
				safe_free_str(&hdoc_string);
				sh->exit_code = EXIT_SIGINT;
				return (NULL);
			}
			return (hdoc_string);
		}
		if (hdoc_add_line(sh, redir, &hdoc_line, &hdoc_string) == EXIT_FAILURE)
			break ;
	}
	return (hdoc_string);
}

int	hdoc_err(int *tmp_fd, char *hdoc_str, char *error_msg)
{
	safe_close_fd(tmp_fd);
	safe_free_str(&hdoc_str);
	msg_perr(error_msg);
	return (EXIT_FAILURE);
}
