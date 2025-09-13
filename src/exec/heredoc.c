/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/11 16:56:39 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

char	*heredoc_loop(t_msh *sh, t_redir *redir)
{
	char	*hdoc_line;
	char	*hdoc_string;
	char	*temp_hdoc_string;
	char	*buffer;
	size_t	len;
	ssize_t	read_bytes;

	hdoc_string = NULL;
	buffer = NULL;
	while (true)
	{
		if (sh->is_interactive)
		{
			hdoc_line = readline("hdoc > ");
			if (!hdoc_line)
			{
				// EOF reached, exit heredoc
				break ;
			}
		}
		else
		{
			// Non-interactive mode: read from stdin
			len = 0;
			read_bytes = getline(&buffer, &len, stdin);
			if (read_bytes == -1)
			{
				// EOF reached, exit heredoc
				free(buffer);
				break ;
			}
			if (read_bytes > 0 && buffer[read_bytes - 1] == '\n')
				buffer[read_bytes - 1] = '\0';
			hdoc_line = ft_strdup(buffer);
			free(buffer);
			buffer = NULL;
			if (!hdoc_line)
			{
				perror("heredoc_loop strdup failed");
				break ;
			}
		}
		if (!*hdoc_line)
		{
			// Empty line, skip it
			safe_free_string(&hdoc_line);
			continue ;
		}
		if (ft_strncmp(redir->string, hdoc_line, ft_strlen(redir->string)) == 0
			&& ft_strlen(redir->string) == ft_strlen(hdoc_line))
		{
			// Found delimiter, free the line and exit
			safe_free_string(&hdoc_line);
			break ;
		}
		if (hdoc_string)
			temp_hdoc_string = ft_strjoin3(hdoc_string, "\n", hdoc_line);
		else
			temp_hdoc_string = ft_strdup(hdoc_line);
		safe_free_string(&hdoc_string);
		safe_free_string(&hdoc_line);
		hdoc_string = temp_hdoc_string;
		temp_hdoc_string = NULL;
		if (hdoc_string == NULL)
		{
			perror("heredoc_loop allocation");
			break ;
		}
	}
	return (hdoc_string);
}

void	heredoc_redirection(t_msh *sh, t_redir *redir, int previous_hdoc_fd)
{
	char	*hdoc_string;
	int		write_fd;

	if (redir && redir->ty == REDIR_HEREDOC)
	{
		if (previous_hdoc_fd)
			close(previous_hdoc_fd);
		write_fd = open("/tmp/myshell_tmp_heredoc",
				O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (write_fd == -1)
			return (perror("open myshell_tmp_heredoc"));
		redir->fd = open("/tmp/myshell_tmp_heredoc", O_RDONLY);
		if (redir->fd == -1)
		{
			close(write_fd);
			return (perror("open heredoc for reading failed"));
		}
		unlink("/tmp/myshell_tmp_heredoc");
		hdoc_string = heredoc_loop(sh, redir);
		if (hdoc_string == NULL)
		{
			hdoc_string = ft_strdup("");
			if (hdoc_string == NULL)
			{
				close(write_fd);
				close(redir->fd);
				sh->exit_code = EXIT_FAILURE;
				perror("heredoc empty string allocation failed");
				return ;
			}
		}
		// Write heredoc content
		if (write(write_fd, hdoc_string, ft_strlen(hdoc_string)) == -1)
		{
			close(write_fd);
			close(redir->fd);
			safe_free_string(&hdoc_string);
			sh->exit_code = EXIT_FAILURE;
			perror("heredoc write failed");
			return ;
		}
		// Add trailing newline to heredoc content (bash compatibility)
		// Only add newline if heredoc is not empty
		if (ft_strlen(hdoc_string) > 0 && write(write_fd, "\n", 1) == -1)
		{
			close(write_fd);
			close(redir->fd);
			safe_free_string(&hdoc_string);
			sh->exit_code = EXIT_FAILURE;
			perror("heredoc write failed");
			return ;
		}
		// Add trailing newline to heredoc content (bash compatibility)
		// Only add newline if heredoc is not empty
		if (ft_strlen(hdoc_string) > 0 && write(write_fd, "\n", 1) == -1)
		{
			close(write_fd);
			close(redir->fd);
			safe_free_string(&hdoc_string);
			sh->exit_code = EXIT_FAILURE;
			perror("heredoc write failed");
			return ;
		}
		close(write_fd);
		safe_free_string(&hdoc_string);
		heredoc_redirection(sh, redir->next, redir->fd);
	}
	else if (redir)
		heredoc_redirection(sh, redir->next, previous_hdoc_fd);
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	heredoc_redirection(sh, cmd->redir, 0);
	return (EXIT_SUCCESS);
}

int	heredoc_pipe_node(t_msh *sh, t_pipe *pipe_node)
{
	sh->exit_code = heredoc_cmd_node(sh, &pipe_node->left->cmd);
	if (sh->exit_code)
		return (sh->exit_code);
	sh->exit_code = heredoc_ast_node(sh, pipe_node->right);
	return (sh->exit_code);
}

int	heredoc_ast_node(t_msh *sh, t_ast *node)
{
	if (!node)
	{
		put_stderr("error: on execute_ast_node_heredoc, ast node is NULL");
		return (EXIT_FAILURE);
	}
	if (node->nty == NODE_CMD)
		sh->exit_code = heredoc_cmd_node(sh, &node->cmd);
	else if (node->nty == NODE_PIPE)
		sh->exit_code = heredoc_pipe_node(sh, &node->pipe);
	return (sh->exit_code);
}
