/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 09:10:35 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/30 16:19:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/*
TODO: check if regular readline will work fine.
otherwise, get this code back, to run with /dev/tty instead of stdin/stdout:
	// if (!readline_on_tty(MINISHELL_PROMPT, &line))
	// 	return (ft_putstr_fd("terminal (tty) not available\n", \
	// 		STDERR_FILENO), EXIT_FAILURE);
*/

int	MINI_SIGNAL = 0;

int	main(int argc, char **argv, char **envp)
{
	t_msh	sh;

	(void)argc;
	(void)argv;
	if (envp[0] == NULL)
	{
		printf("empty environment variable\n");
		return (EXIT_FAILURE);
	}
	if (initialize_minishell(&sh, envp) != EXIT_SUCCESS)
	{
		printf("TODO: handle shell initialize error here");
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
		printf("TODO: Out of memory error here");
		return (ENOMEM);
	}
	return (EXIT_SUCCESS);
}

int	expand_line(t_msh *sh)
{
	if (ft_strlen(sh->line) == 0)
		return (EXIT_FAILURE);
	printf("line before expanding is:%s\n", sh->line);
	return (EXIT_SUCCESS);
}

void	ctrl_c(int sig)
{
	(void)sig;
	MINI_SIGNAL = 1;
	write(1, "\n", 1); // Print a newline
	rl_replace_line("", 0); // Clear the current input line
	rl_on_new_line();       // Move to a new line
	rl_redisplay();         // Redisplay the prompt
}

int	minishell_mainloop(t_msh *sh)
{
   struct sigaction sa;

	rl_catch_signals = 0;
	sa.sa_handler = ctrl_c;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	while (1)
	{
		sh->line = readline(MINISHELL_PROMPT);
		if (!sh->line)
		{
			write(1, "exit\n", 5);
			sh->exit_code = EXIT_SUCCESS;
			free(sh->line);
			break ;
		}
		if (MINI_SIGNAL == 1)
			continue ;
		if (*sh->line)
			add_history(sh->line);
		if (expand_line(sh) != EXIT_SUCCESS)
		{
			printf("TODO: Error expanding line here");
			safe_free_string(&sh->line);
			continue ;
		}
		if (ft_strncmp(sh->line, "exit", 4) == 0)
		{
			// ADD CHECK FOR PIPE, with pipe it does not exit!
			sh->exit_code = EXIT_SUCCESS;
			break ;
		}
		if (parse_line_and_execute_ast(sh) != EXIT_SUCCESS)
		{
		//	printf("TODO: handle parse error here\n");
			safe_free_string(&sh->line);
			continue ;
		}
		free_ast(sh->ast);
		safe_free_string(&sh->line);
	}
	free_everything(sh);
	return (sh->exit_code);
}

void	free_everything(t_msh *sh)
{
	free_ast(sh->ast);
	safe_free_string(&sh->line);
	safe_free_2d_string(&sh->argv);
	rl_clear_history();
}
