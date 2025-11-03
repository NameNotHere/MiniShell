/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_errors.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:56:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/03 11:50:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ERRORS_H
# define MINISHELL_ERRORS_H

/*
	IMPORTANT: for long messages: break with \ char please (no adjacent strings).
	it must continue from the start of next line, or whitespace is added to msg.
*/


/*
	ERRNO CODE used for minishell functions that try to use the errno value.
	If no errno found, fallsback to EXIT_FAILURE (1)
*/
# define ERRNO_CODE -1

/* minishell start error message */
# define E_MINISHELL "minishell: "

/* minishell full error messages */
# define E_SEMICOLON "syntax error near unexpected token ';'"
# define E_UNCLOSED_QUOTES "unclosed quotes"
# define E_REDIR_INVALID "syntax error near unexpected token"
# define E_REDIR_ALLOCATION "redirection allocation failed"
# define E_INIT_ENV "initialize_environment allocation failed"
# define E_OPTION_C_ARGUMENT "-c: option requires an argument"
# define E_CD_ALLOC "cd: memory allocation failure"

/* minishell partial error messages */
# define E_EXPORT_START "export: `"
# define E_EXPORT_END "': not a valid identifier"

#endif