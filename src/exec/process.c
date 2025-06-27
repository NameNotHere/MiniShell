/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:59:07 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/27 06:25:18 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
		MINISHELL EXECUTION CODE COMMENTED OUT BELOW
*/
// void	execute_cmd_node(t_msh *sh, t_cmd *cmd, bool from_pipe)
// {
// 	printf("execute cmd");
// }

// void	execute_ast_node(t_msh *sh, t_ast *node, bool from_pipe)
// {
// 	if (!node)
// 	{
// 		printf("error: on execute, ast node is NULL");
// 		return ;
// 	}
// 	if (node->nty == NODE_CMD)
// 		execute_cmd_node(sh, &node->cmd, from_pipe);
// 	else if (node->nty == NODE_PIPE)
// 	{
// 		execute_ast_node(sh, node->pipe.left, true);
// 		execute_ast_node(sh, node->pipe.right, true);
// 	}
// }
/*
		PIPEX EXECUTION CODE COMMENTED OUT BELOW
*/
// int	handle_execute_command_errors(t_pipex *px, size_t cmd_i)
// {
// 	if (errno == EACCES)
// 	{
// 		if (px->cmd_arg[cmd_i][0] == NULL)
// 			put_stderr("permission denied: (empty command)\n");
// 		else
// 			put_stderr_3("permission denied: ", px->cmd_arg[cmd_i][0], "\n");
// 		return (126);
// 	}
// 	if (px->cmd_arg[cmd_i][0] == NULL)
// 		put_stderr("command not found: (empty command)\n");
// 	else
// 		put_stderr_3("command not found: ", px->cmd_arg[cmd_i][0], "\n");
// 	return (127);
// }

// int	execute_command(t_pipex *px, size_t cmd_i)
// {
// 	if (px->cmd_not_found[cmd_i])
// 	{
// 		if (px->cmd_arg[cmd_i][0] == NULL)
// 			put_stderr("command not found: (empty command)\n");
// 		else
// 			put_stderr_3("command not found: ", px->cmd_arg[cmd_i][0], "\n");
// 		return (127);
// 	}
// 	if (px->fdin != STDIN_FILENO)
// 	{
// 		if (dup2(px->fdin, STDIN_FILENO) == -1)
// 		{
// 			put_stderr("error: failed to redirect input\n");
// 			return (1);
// 		}
// 		close(px->fdin);
// 	}
// 	execve(px->cmd_path[cmd_i], px->cmd_arg[cmd_i], px->envp);
// 	return (handle_execute_command_errors(px, cmd_i));
// }

// void	process_piped_command(t_pipex *px, size_t cmd_i)
// {
// 	int		pipefd[2];
// 	pid_t	pid;

// 	if (pipe(pipefd) == -1)
// 		exit_error_free(px, "pipe");
// 	pid = fork();
// 	if (pid == -1)
// 		close_fds_exit_error_free(px, "fork", pipefd);
// 	if (pid == 0)
// 	{
// 		close(pipefd[0]);
// 		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 			close_fds_exit_error_free(px,
// 				"error: failed to redirect output", pipefd);
// 		close(pipefd[1]);
// 		px->exit_code = execute_command(px, cmd_i);
// 		exit_free_with_code(px, px->exit_code);
// 	}
// 	close(pipefd[1]);
// 	if (px->fdin != STDIN_FILENO)
// 		close(px->fdin);
// 	px->fdin = pipefd[0];
// }

// pid_t	process_last_command(t_pipex *px, size_t cmd_i)
// {
// 	pid_t	pid;

// 	px->fdout = open(px->outfile, px->outfile_flags, PIPEX_CREATE_PERMISSIONS);
// 	if (px->fdout == -1)
// 		exit_error_free(px, px->outfile);
// 	pid = fork();
// 	if (pid == -1)
// 		exit_error_free(px, "fork");
// 	if (pid == 0)
// 	{
// 		if (dup2(px->fdout, STDOUT_FILENO) == -1)
// 		{
// 			put_stderr("error: failed to redirect output\n");
// 			close(px->fdout);
// 			exit_free_with_code(px, EXIT_FAILURE);
// 		}
// 		close(px->fdout);
// 		px->exit_code = execute_command(px, cmd_i);
// 		exit_free_with_code(px, px->exit_code);
// 	}
// 	if (px->fdin != STDIN_FILENO)
// 		close(px->fdin);
// 	close(px->fdout);
// 	return (pid);
// }
