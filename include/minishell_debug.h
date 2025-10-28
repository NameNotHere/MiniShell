/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_debug.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:00:18 by tda-roch          #+#    #+#             */
/*   Updated: 2025/10/25 05:25:59 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_DEBUG_H
# define MINISHELL_DEBUG_H

# include <stdio.h>
# include <stdarg.h>

// forward declarations:
// typedef struct s_ast	t_ast;
// typedef struct s_msh	t_msh;
// typedef struct s_redir	t_redir;
// typedef struct s_token	t_token;
// typedef enum e_redir_ty	t_redir_ty;

// debug/utils_debug.c
void	temp_print(const char *str, ...);

#endif