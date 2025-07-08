/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 17:35:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/09 01:19:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	failed_open_to_null(t_msh *sh, char *filename, int o_flag)
{
	int	new_fd;

	perror(filename);
	new_fd = open("/dev/null", o_flag);
	if (new_fd == -1)
		exit_error_free(sh, "failed to open /dev/null");
	sh->exit_code = EXIT_FAILURE;
	return (new_fd);
}


int	open_input_redirection(t_msh *sh, char *filename)
{
	int	new_fd;

	new_fd = open(filename, O_RDONLY);
	if (new_fd == -1)
		new_fd = failed_open_to_null(sh, filename, O_RDONLY);
	return (new_fd);
}

int	open_output_redirection(t_msh *sh, char *filename)
{
	int	new_fd;

	new_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, OUTPUT_PERMISSIONS);
	if (new_fd == -1)
		new_fd = failed_open_to_null(sh, filename, O_WRONLY);
	return (new_fd);
}

int	open_append_redirection(t_msh *sh, char *filename)
{
	int	new_fd;

	new_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, OUTPUT_PERMISSIONS);
	if (new_fd == -1)
		new_fd = failed_open_to_null(sh, filename, O_WRONLY);
	return (new_fd);
}

void	execute_redirection_in(t_msh *sh, t_redir *redir)
{
	int	new_fd;
	int	fd_out;

	fd_out = STDOUT_FILENO;
	if (redir->ty == REDIR_HEREDOC)
		new_fd = redir->fd;
	else if (redir->ty == REDIR_INPUT)
		new_fd = open_input_redirection(sh, redir->string);
	if (new_fd != -1)
	{
		try_dup2_stdin(sh, &new_fd, &fd_out);
		safe_close_fd_in(&new_fd);
	}
}

void	execute_redirection_out(t_msh *sh, t_redir *redir)
{
	int	fd_in;
	int	new_fd;

	fd_in = STDIN_FILENO;
	if (redir->ty == REDIR_OUTPUT)
		new_fd = open_output_redirection(sh, redir->string);
	else if (redir->ty == REDIR_APPEND)
		new_fd = open_append_redirection(sh, redir->string);
	if (new_fd != -1)
	{
		try_dup2_stdout(sh, &fd_in, &new_fd);
		safe_close_fd_out(&new_fd);
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

int	execute_command(t_msh *sh, t_cmd *cmd)
{
	if (cmd->not_found)
	{
		if (cmd->argv[0] == NULL)
			put_stderr("command not found: (empty command)\n");
		else
			put_stderr_3("command not found: ", cmd->argv[0], "\n");
		sh->exit_code = 127;
		return (127);
	}
	execve(cmd->full_cmd, cmd->argv, sh->envp);
	return (handle_execute_command_errors(sh, cmd));
}

int	execute_cmd_node(t_msh *sh, t_cmd *cmd, int fd_in, int fd_out)
{
	pid_t	pid;

	sh->last_pid = fork();
	pid = sh->last_pid;
	if (pid == -1)
	{
		safe_close_fds(&fd_in, &fd_out);
		perror("fork");
		sh->exit_code = errno;
		return (sh->exit_code);
	}
	if (pid == 0)
	{
		try_dup2(sh, &fd_in, &fd_out);
		safe_close_fds(&fd_in, &fd_out);
		execute_redirection(sh, cmd->redir);
		sh->exit_code = execute_command(sh, cmd);
		exit_free_with_code(sh, sh->exit_code);
	}
	debug_print_one_redir(cmd->redir);
	safe_close_fd_out(&fd_out);
	return (EXIT_SUCCESS);
}
