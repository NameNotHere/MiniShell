/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:22:23 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 23:57:20 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	execute_redirection_in(t_msh *sh, t_redir *redir)
{
	int	fd;

	fd = STDIN_FILENO;
	if (redir->ty == REDIR_HEREDOC)
		fd = redir->fd;
	else if (redir->ty == REDIR_INPUT)
		fd = open_input_redirection(sh, redir->string);
	if (fd == -1)
		return (false);
	try_dup2_stdin(sh, &fd);
	safe_close_fd(&fd);
	return (true);
}

bool	execute_redirection_out(t_msh *sh, t_redir *redir)
{
	int	fd;

	fd = STDOUT_FILENO;
	if (redir->ty == REDIR_OUTPUT)
		fd = open_output_redirection(sh, redir->string);
	else if (redir->ty == REDIR_APPEND)
		fd = open_append_redirection(sh, redir->string);
	if (fd == -1)
		return (false);
	try_dup2_stdout(sh, &fd);
	safe_close_fd(&fd);
	return (true);
}

/*
	Execute each redirection recursively until redirection list ends.
	Returns true if all redirections succeed, false if any fail.
*/
bool	execute_redirection(t_msh *sh, t_redir *redir)
{
	if (redir && redir->string)
	{
		if (redir->ty == REDIR_INPUT || redir->ty == REDIR_HEREDOC)
		{
			if (!execute_redirection_in(sh, redir))
				return (false);
		}
		else if (redir->ty == REDIR_OUTPUT || redir->ty == REDIR_APPEND)
		{
			if (!execute_redirection_out(sh, redir))
				return (false);
		}
		return (execute_redirection(sh, redir->next));
	}
	return (true);
}
