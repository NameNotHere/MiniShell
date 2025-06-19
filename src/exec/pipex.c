/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 13:21:53 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/15 20:37:52 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	process_all_piped_commands(t_pipex *px, size_t cmd_i)
{
	int		child_status;
	pid_t	child_pid;
	pid_t	last_cmd_pid;
	int		last_exit_status;

	last_exit_status = px->exit_code;
	while (cmd_i < px->cmd_total - 1)
		process_piped_command(px, cmd_i++);
	last_cmd_pid = process_last_command(px, cmd_i);
	child_pid = 1;
	while (child_pid > 0)
	{
		child_pid = wait(&child_status);
		if (child_pid == last_cmd_pid)
		{
			if (WIFEXITED(child_status))
				last_exit_status = WEXITSTATUS(child_status);
			else if (WIFSIGNALED(child_status))
				last_exit_status = 128 + WTERMSIG(child_status);
		}
	}
	if (child_pid == -1 && (errno != ECHILD && errno != EINTR))
		exit_error_free(px, "wait error");
	free_everything(px);
	return (last_exit_status);
}

int	run_pipex_once(int argc, char **argv, char **envp)
{
	t_pipex	px;
	size_t	cmd_i;

	if (argc < 5 || (ft_strncmp(argv[1], "here_doc", 8) == 0 && argc < 6))
	{
		put_stderr_2("Not enough arguments", "\n");
		return (1);
	}
	initialize_pipex(&px, argc, argv, envp);
	initialize_pipex_commands(&px);
	cmd_i = 0;
	while (cmd_i < px.cmd_total)
		parse_command(&px, cmd_i++);
	cmd_i = 0;
	return (process_all_piped_commands(&px, cmd_i));
}

int	main(int argc, char **argv, char **envp)
{
	if (argc == 2 && (ft_strncmp(argv[1], "-i", 2) == 0))
		return (run_pipex_interactive(argv[0], envp));
	else
		return (run_pipex_once(argc, argv, envp));
}
