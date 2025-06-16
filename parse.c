
#include "minishell.h"

int	skip_spaces(int *i, char *str)
{
	int	y;

	y = 0;
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\n' || str[*i] == '\t'))
	{
		(*i)++;
		y++;
	}
	return (y);
}

int count_tokens(char *str)
{
	int		count = 0;
	int		i = 0;
	char	quote;
	while (str[i])
	{
		skip_spaces(&i, str);
		if (str[i])
		{
			count++;
			quote = str[i];
			if (str[i] == '\'' || str[i] == '\"')
				while (str[i++] && str[i] != quote)
					count = count;
			else
				while (str[i] && !ft_isspace(str[i]) && str[i] != '\'' && str[i] != '\"')
	   				i++;
		}
	}
	return (count);
}

int	is_builtin(char *str)
{
	if (!str)
		return (0);
	if (ft_strncmp(str, "ls", 2) == 0)
		return (1);
	else if (ft_strncmp(str, "cd", 2) == 0)
		return (1);
	else if (ft_strncmp(str, "echo", 4) == 0)
		return (1);
	else if (ft_strncmp(str, "pwd", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "export", 5) == 0)
		return (1);
	else if (ft_strncmp(str, "unset", 5) == 0)
		return (1);
	else if (ft_strncmp(str, "env", 3) == 0)
		return (1);
	else if (ft_strncmp(str, "exit", 4) == 0)
		return (1);
	return (0);
}

void make_string(char *str, int *len, int i)
{
	char	quote;
	int 	l;

	quote = str[i];
	l = 1;
	i++;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			l += 2;
		}
		else if (str[i] == quote)
		{
			l++;
			i++;
			break;
		}
		else
		{
			i++;
			l++;
		}
	}
	*len = l;
}


void parse_word(char *str, int i, int *len)
{
	while (str[i] && !ft_isspace(str[i]) && str[i] != '\'' && str[i] != '\"' && !ft_minishellop(str, i))
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i += 2;
			*len += 2;
		}
		else
		{
			i++;
			(*len)++;
		}
	}
}

char *make_word(char *str, int *i)
{
	int len;
	char *word;

	skip_spaces(i, str);
	len = 0;
	if (str[*i] && (str[*i] == '\'' || str[*i] == '\"'))
		make_string(str, &len, *i);	
	else if (str[*i] && (ft_isalpha(str[*i]) || str[*i] == '.'))
		parse_word(str, *i, &len);
	else if (str[*i] && ft_minishellop(str, *i))
		len += ft_minishellop(str, *i);
	else if (str[*i] && ft_isdigit(str[*i]))
		while (ft_isdigit(str[*i + len]))
			len++;
	(*i) += len;
	if (!str || !(word = malloc(len + 1)))
		return NULL;
	ft_memcpy(word, str + (*i - len), len);
	word[len] = '\0';
	return (word);
}
