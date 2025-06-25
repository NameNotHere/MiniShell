/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:51:03 by tda-roch          #+#    #+#             */
/*   Updated: 2025/06/25 14:59:00 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"
# include "minishell_parser.h"

# define MINISHELL_PROMPT "minishell> "

// utils_free.c

void	safe_free(char **ptr);

void	safe_free_2d(char ***ptr);

void	safe_free_3d(char ****ptr);

void	safe_free_bool(bool **ptr);

// utils_readine

bool	readline_on_tty(const char *prompt, char **line);

#endif