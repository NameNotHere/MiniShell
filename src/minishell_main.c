/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/10 18:28:41 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

// user defined variable (including global) must be lowercase.
//global must start with g_
int	g_mini_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	t_msh				sh;
	struct sigaction	sa;

	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	sa.sa_handler = ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	if (envp[0] == NULL)
	{
		d_print("empty environment variable\n");
		return (EXIT_FAILURE);
	}
	if (initialize_minishell(&sh, envp) != EXIT_SUCCESS)
	{
		d_print("TODO: handle shell initialize error here");
		return (sh.exit_code);
	}
	if (minishell_mainloop(&sh) != EXIT_SUCCESS)
		return (sh.exit_code);
	return (EXIT_SUCCESS);
}

/*
TODO: handle ENOMEM (Out of memory) error for path dirs
*/
int	initialize_minishell(t_msh *sh, char **envp)
{
	ft_bzero(sh, sizeof(*sh));
	sh->envp = copy_string_array(envp);
	sh->ast = make_ast_node(NODE_UNKNOWN);
	sh->path_dirs = ft_split(get_path_from_env(sh->envp), ':');
	if (!sh->path_dirs || !sh->envp)
	{
		d_print("TODO: Out of memory error here");
		return (ENOMEM);
	}
	return (EXIT_SUCCESS);
}

void	ctrl_c(int sig)
{
	(void)sig;
	g_mini_signal = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// int	minishell_mainloop(t_msh *sh)
// {
// 	while (true)
// 	{
// 		sh->line = readline(MINISHELL_PROMPT);
// 		if (!sh->line || !*sh->line)
// 		{
// 			safe_free_string(&sh->line);
// 			continue ;
// 		}
// 		add_history(sh->line);
// 		if (expand_line(sh) != EXIT_SUCCESS)
// 		{
// 			d_print("TODO: Error expanding line here");
// 			safe_free_string(&sh->line);
// 			continue ;
// 		}
// 		if (ft_strncmp(sh->line, "exit", 4) == 0 && !piped_line(sh->line))
// 		{
// 			sh->exit_code = EXIT_SUCCESS;
// 			break ;
// 		}
// 		if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
// 		{
// 			perror("parse line");
// 			safe_free_string(&sh->line);
// 			continue ;
// 		}
// 		if (lookup_all_cmd_fullpaths(sh, sh->ast) != EXIT_SUCCESS)
// 			perror("lookup_cmds");
// 		if (execute_ast_root(sh, sh->ast,
// 				STDIN_FILENO, STDOUT_FILENO) != EXIT_SUCCESS)
// 			perror("exec ast root");
// 		free_ast(&sh->ast);
// 		sh->ast = make_ast_node(NODE_UNKNOWN);
// 		safe_free_string(&sh->line);
// 	}
// 	free_everything(sh);
// 	return (sh->exit_code);
// }

int	minishell_mainloop(t_msh *sh)
{
	bool	is_interactive;
	char	*buffer;
	size_t	len;
	ssize_t	read_bytes;

	is_interactive = isatty(STDIN_FILENO);
	buffer = NULL;
	while (true)
	{
		if (is_interactive)
		{
			sh->line = readline(MINISHELL_PROMPT);
			if (!sh->line)
				break ; // EOF = exit
		}
		else
		{
			len = 0;
			read_bytes = getline(&buffer, &len, stdin); // TODO: check.
			if (read_bytes == -1)
			{
				free(buffer);
				break ; // EOF = exit
			}
			if (read_bytes > 0 && buffer[read_bytes - 1] == '\n')
				buffer[read_bytes - 1] = '\0';
			sh->line = buffer;
		}
		if (!sh->line || !*sh->line)
		{
			safe_free_string(&sh->line);
			continue ;
		}
		if (is_interactive)
			add_history(sh->line);
		if (expand_line(sh) != EXIT_SUCCESS)
		{
			d_print("TODO: Error expanding line here");
			safe_free_string(&sh->line);
			continue ;
		}
		if (ft_strncmp(sh->line, "exit", 4) == 0 && !piped_line(sh->line))
		{
			sh->exit_code = EXIT_SUCCESS;
			break ;
		}
		if (parse_line_to_ast(sh, sh->ast, sh->line) != EXIT_SUCCESS)
		{
			perror("parse line");
			safe_free_string(&sh->line);
			continue ;
		}
		if (lookup_all_cmd_fullpaths(sh, sh->ast) != EXIT_SUCCESS)
			perror("lookup_cmds");
		if (execute_ast_root(sh, sh->ast,
				STDIN_FILENO, STDOUT_FILENO) != EXIT_SUCCESS)
			perror("exec ast root");
		free_ast(&sh->ast);
		sh->ast = make_ast_node(NODE_UNKNOWN);
		safe_free_string(&sh->line);
	}
	free_everything(sh);
	return (sh->exit_code);
}



void	free_everything(t_msh *sh)
{
	free_ast(&sh->ast);
	safe_free_string(&sh->line);
	safe_free_2d_string(&sh->envp);
	rl_clear_history();
}
