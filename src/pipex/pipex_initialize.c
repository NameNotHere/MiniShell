/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_initialize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:36:32 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/15 20:40:17 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	initialize_pipex(t_pipex *px, int argc, char **argv, char **envp)
{
	ft_bzero(px, sizeof(*px));
	px->infile = argv[1];
	px->outfile = argv[argc - 1];
	px->argc = argc;
	px->argv = argv;
	px->envp = envp;
	px->hdoc = ft_strncmp(px->infile, "here_doc", 8) == 0;
	if (px->hdoc)
	{
		px->outfile_flags = O_WRONLY | O_CREAT | O_APPEND;
		heredoc_pipe(px);
	}
	else
	{
		px->outfile_flags = O_WRONLY | O_CREAT | O_TRUNC;
		open_infile(px);
	}
}

void	initialize_pipex_commands(t_pipex *px)
{
	px->cmd_offset = 2 + px->hdoc;
	px->cmd_total = px->argc - px->cmd_offset - 1;
	px->cmd_arg = ft_calloc((px->cmd_total + 1), sizeof(char **));
	if (!px->cmd_arg)
		exit_error_free(px, "memory allocation failed for command arguments");
	px->cmd_path = ft_calloc((px->cmd_total + 1), sizeof(char *));
	if (!px->cmd_path)
		exit_error_free(px, "memory allocation failed for command paths");
	px->cmd_not_found = ft_calloc((px->cmd_total + 1), sizeof(bool));
	if (!px->cmd_not_found)
		exit_error_free(px, "memory allocation failed for not_found flags");
	px->path_dirs = split_single_delimiter(get_path_from_env(px->envp), ':');
	if (!px->path_dirs)
		exit_error_free(px, "memory allocation failed for path diretories");
}

void	open_infile(t_pipex *px)
{
	px->fdin = open(px->infile, O_RDONLY);
	if (px->fdin == -1)
	{
		if (errno)
			perror(px->infile);
		else
			put_stderr_2(px->infile, "\n");
		px->fdin = open("/dev/null", O_RDONLY);
		if (px->fdin == -1)
			exit_error_free(px, "failed to open /dev/null");
		px->exit_code = 1;
	}
}
