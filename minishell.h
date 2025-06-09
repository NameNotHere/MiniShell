/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/09 13:19:18 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_VARIABLE,
	TOKEN_PARAM
}	t_token_type;

typedef struct t_token
{
	e_token_type	ty;
	char			*word;
}	t_token;

int		count_tokens(char *str);

char	*make_word(char *str);

t_token	token(char *str);

t_token	*tokenize(char *input);

int		strcmp(char *str, char *str1);

int parse_tokens(t_token *list);
