/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 17:00:32 by otanovic          #+#    #+#             */
/*   Updated: 2025/06/18 12:50:03 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_INBUILT,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SINGLE_QUOTE,
	TOKEN_DOUBLE_QUOTE,
	TOKEN_VARIABLE,
	TOKEN_PARAM,
	TOKEN_FILE_PATH,
	TOKEN_NUMBER,
	TOKEN_BACKSLASH
}	t_token_type;

typedef struct t_token
{
	t_token_type	ty;
	char			*word;
}	t_token;

int		count_tokens(char *str);

t_token	token(char *str);

t_token	*tokenize(char *input, int *token_count);

int		parse_tokens(t_token *list);

int		skip_spaces(int *i, char *str);

int		is_builtin(char *str);

char	*make_word(char *str, int *i);

int		ft_atoi(const char *s);

void	*ft_calloc(size_t nmemb, size_t size);

int		ft_isalnum(int c);

int		ft_isalpha( int c );

int		ft_isascii(int c);

int		ft_isdigit(int c);

int		ft_isminioperator(char *token, int i);

int		ft_isprint(int c);

int		ft_isspace(char c);

size_t	ft_strlen(const char *str);

int		ft_strncmp(const char *s1, const char *s2, size_t n);

void	*ft_memcpy(void *dst, const void *src, size_t num);
