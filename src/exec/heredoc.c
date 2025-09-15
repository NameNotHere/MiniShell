/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 02:06:41 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/15 02:35:18 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>

/*
	Adds a line to a string, after newline char.
	If string is NULL, string is copy of the line.
*/
int	add_line_to_string(char **string, char **line)
{
	int		result;
	char	*updated_string;

	result = EXIT_SUCCESS;
	if (!(*line) || !*(*line))
	{
		put_stderr("add line to string: invalid line");
		return (EXIT_FAILURE);
	}
	if (*string)
		updated_string = ft_strjoin3(*string, "\n", *line);
	else
		updated_string = ft_strdup(*line);
	if (updated_string == NULL)
	{
		perror("add line to string");
		result = EXIT_FAILURE;
	}
	safe_free_string(string);
	safe_free_string(line);
	*string = updated_string;
	updated_string = NULL;
	return (result);
}

char	*get_hdoc_line(t_msh *sh)
{
	char	*hdoc_line;
	size_t	cap;
	ssize_t	read_bytes;

	hdoc_line = NULL;
	if (sh->is_interactive)
		hdoc_line = readline("hdoc > ");
	else
	{
		cap = 0;
		read_bytes = getline(&hdoc_line, &cap, stdin);
		if (read_bytes == -1)
		{
			safe_free_string(&hdoc_line);
			return (NULL);
		}
		if (read_bytes > 0 && hdoc_line[read_bytes - 1] == '\n')
			hdoc_line[read_bytes - 1] = '\0';
	}
	return (hdoc_line);

}

char	*hdoc_loop(t_msh *sh, t_redir *redir)
{
	char	*hdoc_line;
	char	*hdoc_string;

	hdoc_string = NULL;
	while (true)
	{
		hdoc_line = get_hdoc_line(sh);
		if (!hdoc_line || !*hdoc_line)
		{
			safe_free_string(&hdoc_line);
			continue ;
		}
		if (ft_strncmp(redir->string, hdoc_line, ft_strlen(redir->string)) == 0
			&& ft_strlen(redir->string) == ft_strlen(hdoc_line))
		{
			safe_free_string(&hdoc_line);
			break ;
		}
		if (add_line_to_string(&hdoc_string, &hdoc_line) == EXIT_FAILURE)
		{
			sh->exit_code = EXIT_FAILURE;
			break ;
		}
	}
	return (hdoc_string);
}

void	hdoc_err(t_msh *sh, int *write_fd, int *redir_fd, char *hdoc_str)
{
	safe_close_2_fds(write_fd, redir_fd);
	safe_free_string(&hdoc_str);
	sh->exit_code = EXIT_FAILURE;
	put_stderr("heredoc redir failed");
	return ;
}

void	hdoc_redir(t_msh *sh, t_redir *redir, int prev_hdoc_fd)
{
	char	*hdoc_str;
	int		write_fd;

	if (!redir)
		return ;
	if (redir && redir->ty != REDIR_HEREDOC)
		return (hdoc_redir(sh, redir->next, prev_hdoc_fd));
	safe_close_fd(&prev_hdoc_fd);
	write_fd = open("/tmp/tmp_hdoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (write_fd == -1)
		return (hdoc_err(sh, NULL, NULL, NULL), perror("open hdoc"));
	redir->fd = open("/tmp/tmp_hdoc", O_RDONLY);
	if (redir->fd == -1)
		return (hdoc_err(sh, &write_fd, NULL, NULL), perror("open hdoc"));
	unlink("/tmp/tmp_hdoc");
	hdoc_str = hdoc_loop(sh, redir);
	if (hdoc_str == NULL && set_empty_string(&hdoc_str) == false)
		return (hdoc_err(sh, &write_fd, &redir->fd, hdoc_str), perror("alloc"));
	if ((write(write_fd, hdoc_str, ft_strlen(hdoc_str)) == -1)
		|| (ft_strlen(hdoc_str) > 0 && write(write_fd, "\n", 1) == -1))
		return (hdoc_err(sh, &write_fd, &redir->fd, hdoc_str), perror("write"));
	close(write_fd);
	safe_free_string(&hdoc_str);
	hdoc_redir(sh, redir->next, redir->fd);
}

int	heredoc_cmd_node(t_msh *sh, t_cmd *cmd)
{
	hdoc_redir(sh, cmd->redir, 0);
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
