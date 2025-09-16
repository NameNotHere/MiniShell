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

void	execute_redirection_in(t_msh *sh, t_redir *redir)
{
	int	fd;

	fd = STDIN_FILENO;
	if (redir->ty == REDIR_HEREDOC)
		fd = redir->fd;
	else if (redir->ty == REDIR_INPUT)
		fd = open_input_redirection(sh, redir->string);
	if (fd == -1)
		return ;
	try_dup2_stdin(sh, &fd);
	safe_close_fd(&fd);
}

void	execute_redirection_out(t_msh *sh, t_redir *redir)
{
	int	fd;

	fd = STDOUT_FILENO;
	if (redir->ty == REDIR_OUTPUT)
		fd = open_output_redirection(sh, redir->string);
	else if (redir->ty == REDIR_APPEND)
		fd = open_append_redirection(sh, redir->string);
	if (fd == -1)
		return ;
	try_dup2_stdout(sh, &fd);
	safe_close_fd(&fd);
}

/*
	Execute each redirection recursively until redirection list ends.
*/
void	execute_redirection(t_msh *sh, t_redir *redir)
{
	if (redir && redir->string)
	{
		if (redir->ty == REDIR_INPUT || redir->ty == REDIR_HEREDOC)
			execute_redirection_in(sh, redir);
		else if (redir->ty == REDIR_OUTPUT || redir->ty == REDIR_APPEND)
			execute_redirection_out(sh, redir);
		execute_redirection(sh, redir->next);
	}
}
