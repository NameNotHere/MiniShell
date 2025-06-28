/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:44:57 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/26 14:12:19 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"
#include <stdio.h>
#include <string.h>

// make a function to check the words and tokens
// double array
int	check_token(char *mini_string, char **mini_token)
{
	int token_count;
	int err;

	printf("\nChecking: %s\n", mini_string);
	t_token *test = tokenize(mini_string, &token_count, &err);
	if (!test)
	{
		printf("❌ Tokenize failed!\n");
		return (0);
	}

	printf("Token count: %d\n", token_count);
	for (int j = 0; j < token_count; j++) {
		printf("Token[%d]: '%s'\n", j, test[j].word ? test[j].word : "NULL");
	}

	for (int i = 0; mini_token[i] != NULL; i++)
	{
		if (i >= token_count)
		{
			printf("❌ i (%d) >= token_count (%d)\n", i, token_count);
			return (0);
		}
		if (!test[i].word)
		{
			printf("❌ test[%d].word is NULL\n", i);
			return (0);
		}
		if (strcmp(test[i].word, mini_token[i]) != 0)
		{
			printf("❌ Mismatch: expected '%s' vs got '%s'\n", mini_token[i], test[i].word);
			return (0);
		}
	}

	// Check for extra tokens
	if (token_count > 0 && test[token_count].word != NULL)
	{
		printf("❌ token_count overflow: test[%d].word is not NULL\n", token_count);
		return (0);
	}

	return (1);
}



void	compare_mini_n_bash()
{
	char *commands[20] = {
    "echo Hello World",                 // echo Hello World
    "ls -la",                           // ls -la
    "pwd",                              // pwd
    "cd /tmp",                          // cd /tmp
    "export VAR=value",                // export VAR=value
    "unset VAR",                        // unset VAR
    "env",                              // env
    "cat file.txt",                     // cat file.txt
    "grep 'main' main.c",              // grep 'main' main.c
    "echo $HOME",                       // echo $HOME
    "echo \"Quoted string\"",           // echo "Quoted string"
    "ls | grep txt",                    // ls | grep txt
    "cat < input.txt",                  // cat < input.txt
    "echo 'hello' > out.txt",           // echo 'hello' > out.txt
    "echo test >> out.txt",             // echo test >> out.txt
    "ls -l | grep '^d' > dirs.txt",     // ls -l | grep '^d' > dirs.txt
    "cd .. && pwd",                     // cd .. && pwd
    "mkdir new_folder && cd new_folder",// mkdir new_folder && cd new_folder
    "rm -rf test_folder",               // rm -rf test_folder
    "echo \"VAR=$VAR\"",                // echo "VAR=$VAR"
	};

	// Token arrays
	char *tokens_0[] = {"echo", "Hello", "World", NULL};
	char *tokens_1[] = {"ls", "-la", NULL};
	char *tokens_2[] = {"pwd", NULL};
	char *tokens_3[] = {"cd", "/tmp", NULL};
	char *tokens_4[] = {"export", "VAR=value", NULL};
	char *tokens_5[] = {"unset", "VAR", NULL};
	char *tokens_6[] = {"env", NULL};
	char *tokens_7[] = {"cat", "file.txt", NULL};
	char *tokens_8[] = {"grep", "main", "main.c", NULL};
	char *tokens_9[] = {"echo", "$HOME", NULL};
	char *tokens_10[] = {"echo", "Quoted string", NULL};
	char *tokens_11[] = {"ls", "|", "grep", "txt", NULL};
	char *tokens_12[] = {"cat", "<", "input.txt", NULL};
	char *tokens_13[] = {"echo", "hello", ">", "out.txt", NULL};
	char *tokens_14[] = {"echo", "test", ">>", "out.txt", NULL};
	char *tokens_15[] = {"ls", "-l", "|", "grep", "^d", ">", "dirs.txt", NULL};
	char *tokens_16[] = {"cd", "..", "&&", "pwd", NULL};
	char *tokens_17[] = {"mkdir", "new_folder", "&&", "cd", "new_folder", NULL};
	char *tokens_18[] = {"rm", "-rf", "test_folder", NULL};
	char *tokens_19[] = {"echo", "VAR=$VAR", NULL};

	// Master array of all token arrays
	char **commands_tokens[20] = {
		tokens_0,
		tokens_1,
		tokens_2,
		tokens_3,
		tokens_4,
		tokens_5,
		tokens_6,
		tokens_7,
		tokens_8,
		tokens_9,
		tokens_10,
		tokens_11,
		tokens_12,
		tokens_13,
		tokens_14,
		tokens_15,
		tokens_16,
		tokens_17,
		tokens_18,
		tokens_19
	};
	for (int i = 0; i < 20; i++)
	{
		int result = check_token(commands[i], commands_tokens[i]);
		printf("Command %2d: %-35s - %s\n", i, commands[i], result ? "✅ OK" : "❌ FAIL");
	}

}

int	main(void)
{
	compare_mini_n_bash();
	return (0);
}
