/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 17:13:27 by otanovic          #+#    #+#             */
/*   Updated: 2025/11/12 09:57:05 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>

/* Standard libft functions */
char		*ft_itoa(int n);
char		*ft_strdup(const char *str);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
void		*ft_calloc(size_t nmemb, size_t size);
int			ft_atoi(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);
void		*ft_memcpy(void *dst, const void *src, size_t num);
void		*ft_memchr(const void *s, int c, size_t n);
size_t		ft_strlen(const char *str);
void		ft_bzero(void *s, size_t n);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isspace(char c);

/* Custom functions for minishell */
long long	ft_atoll(const char *s);
int			ft_strcmp(const char *s1, const char *s2);
char		*ft_strjoin3(char const *s1, char const *s2, char const *s3);
char		*ft_strndup(const char *src, int size);
void		*ft_realloc(void *ptr, size_t new_size, size_t old_size);

#endif
