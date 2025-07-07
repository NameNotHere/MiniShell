/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/07/07 22:15:54 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <stdio.h>
#include <string.h>

int	check_token(char *mini_string, char **mini_token)
{
	int	token_count;
	int	err;

    t_print("\nChecking: %s\n", mini_string);
    t_token *test = tokenize(mini_string, &token_count, &err);
    if (!test)
	{
		t_print("❌ Tokenize failed!\n");
		return (0);
	}

	t_print("Token count: %d\n", token_count);
	for (int j = 0; j < token_count; j++) {
		t_print("Token[%d]: '%s' TOKEN TYPE %12s\n", j, test[j].word ? test[j].word : "NULL", get_token_name(test[j].ty));
	}

	int i = 0;
	for (; mini_token[i] != NULL; i++)
	{
		if (i >= token_count)
		{
			t_print("❌ Too few tokens: expected %d got %d\n", i+1,
				token_count);
			return (0);
		}
		if (!test[i].word)
		{
			t_print("❌ test[%d].word is NULL\n", i);
			return (0);
		}
		if (strcmp(test[i].word, mini_token[i]) != 0)
		{
			t_print("❌ Mismatch at token %d: expected '%s' got '%s'\n",
				i, mini_token[i], test[i].word);
			return (0);
		}
	}
	if (token_count > i)
	{
		t_print("❌ Too many tokens: expected %d got %d\n", i,
			token_count);
		return (0);
	}
	return (1);
}

void	compare_mini_n_bash()
{
	char	*commands[30] = {
		"echo Hello World",
		"ls -la",
		"pwd",
		"cd /tmp",
		"export VAR=value",
		"unset VAR",
		"env",
		"cat file.txt",
		"grep 'main' main.c",
		"echo $HOME",
		"echo \"Quoted string\"",
		"ls | grep txt",
		"cat < input.txt",
		"echo 'hello' > out.txt",
		"echo test >> out.txt",
		"ls -l | grep '^d' > dirs.txt",
		"cd .. && pwd",
		"mkdir new_folder && cd new_folder",
		"rm -rf test_folder",
		"echo \"VAR=$VAR\"",
		// New tests:
		"echo \"A \\\"quote\\\" inside\"",
		"echo 'Single quotes with $NO_EXPAND'",
		"echo \"Mixing 'quotes' and $VAR\"",
		"cat << EOF",
		"command arg1; command2 arg2",
		"echo path\\ with\\ spaces",
		"ls -l && echo done || echo failed",
		"echo $((1 + 2))",
		"echo $?",
		"echo `date`"
	};

	char	**commands_tokens[30] = {
		(char *[]){"echo", "Hello", "World", NULL},
		(char *[]){"ls", "-la", NULL},
		(char *[]){"pwd", NULL},
		(char *[]){"cd", "/tmp", NULL},
		(char *[]){"export", "VAR=value", NULL},
		(char *[]){"unset", "VAR", NULL},
		(char *[]){"env", NULL},
		(char *[]){"cat", "file.txt", NULL},
		(char *[]){"grep", "main", "main.c", NULL},
		(char *[]){"echo", "$HOME", NULL},
		(char *[]){"echo", "Quoted string", NULL},
		(char *[]){"ls", "|", "grep", "txt", NULL},
		(char *[]){"cat", "<", "input.txt", NULL},
		(char *[]){"echo", "hello", ">", "out.txt", NULL},
		(char *[]){"echo", "test", ">>", "out.txt", NULL},
		(char *[]){"ls", "-l", "|", "grep", "^d", ">", "dirs.txt", NULL},
		(char *[]){"cd", "..", "&&", "pwd", NULL},
		(char *[]){"mkdir", "new_folder", "&&", "cd", "new_folder", NULL},
		(char *[]){"rm", "-rf", "test_folder", NULL},
		(char *[]){"echo", "VAR=$VAR", NULL},
		// New token arrays inlined:
		(char *[]){"echo", "A \"quote\" inside", NULL},
		(char *[]){"echo", "Single quotes with $NO_EXPAND", NULL},
		(char *[]){"echo", "Mixing 'quotes' and $VAR", NULL},
		(char *[]){"cat", "<<", "EOF", NULL},
		(char *[]){"command", "arg1", ";", "command2", "arg2", NULL},
		(char *[]){"echo", "path with spaces", NULL},
		(char *[]){"ls", "-l", "&&", "echo", "done", "||", "echo", "failed",\
			NULL},
		(char *[]){"echo", "$((1 + 2))", NULL},
		(char *[]){"echo", "$?", NULL},
		(char *[]){"echo", "`date`", NULL}
	};

	for (int i = 0; i < 30; i++)
	{
		int result = check_token(commands[i], commands_tokens[i]);
		t_print("Command %2d: %-40s - %s\n", i, commands[i], result ? "✅ OK" : "❌ FAIL");
	}
}

void	test_tokens(void)
{
	char	*tokens1[] = {"ls", "-lsa", NULL};
	char	*tokens2[] = {"echo", "a", NULL};
	char	*tokens3[] = {"stirng", NULL};
	char	*tokens4[] = {"vqr=$var1", NULL};

	check_token("ls -lsa", tokens1);
	check_token("echo a", tokens2);
	check_token("\"stirng\"", tokens3);
	check_token("vqr=\"$var1\"", tokens4);
}

// write a function to test token fully all cases
int	main(void)
{
	compare_mini_n_bash();
	test_tokens();
	return (0);
}
