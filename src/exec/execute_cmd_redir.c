/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:22:23 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/09 02:16:16 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_redirection_in(t_msh *sh, t_redir *redir)
{
	int	new_fd_in;

	if (redir->ty == REDIR_HEREDOC)
		new_fd_in = redir->fd;
	else if (redir->ty == REDIR_INPUT)
		new_fd_in = open_input_redirection(sh, redir->string);
	if (new_fd_in != -1)
	{
		try_dup2_stdin(sh, &new_fd_in);
		safe_close_fd_in(&new_fd_in);
	}
}

void	execute_redirection_out(t_msh *sh, t_redir *redir)
{
	int	new_fd_out;

	if (redir->ty == REDIR_OUTPUT)
		new_fd_out = open_output_redirection(sh, redir->string);
	else if (redir->ty == REDIR_APPEND)
		new_fd_out = open_append_redirection(sh, redir->string);
	if (new_fd_out != -1)
	{
		try_dup2_stdout(sh, &new_fd_out);
		safe_close_fd_out(&new_fd_out);
	}
}

/*
Execute each redirection recursively until redirection list ends.
*/
void	execute_redirection(t_msh *sh, t_redir *redir)
{
	if (redir && redir->string)
	{
		// if (redir->ty == REDIR_INPUT || redir->ty == REDIR_HEREDOC)
		if (redir->ty == REDIR_INPUT)  // RIGHT NOW, SKIP HEREDOC
			execute_redirection_in(sh, redir);
		else if (redir->ty == REDIR_OUTPUT || redir->ty == REDIR_APPEND)
			execute_redirection_out(sh, redir);
		execute_redirection(sh, redir->next);
	}
}
