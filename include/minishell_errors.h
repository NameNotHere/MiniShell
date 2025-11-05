/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_errors.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:56:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/05 17:10:56 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ERRORS_H
# define MINISHELL_ERRORS_H

/* ERRNO CODE used for minishell functions that try to use the errno value.
	If no errno found, fallsback to EXIT_FAILURE (1) */
# define ERRNO_CODE -1

/* minishell start error message */
# define E_MINISHELL "minishell: "

/* minishell initialization and syntax error messages */
# define E_CD_ALLOC "cd: memory allocation failure"
# define E_CD_CWD_NULL "cd: error retrieving current directory: getcwd: cannot \
access parent directories: No such file or directory"
# define E_EXPORT_END "': not a valid identifier"
# define E_EXPORT_START "export: `"
# define E_INIT_ENV "initialize_environment allocation failed"
# define E_OPTION_C_ARGUMENT "-c: option requires an argument"
# define E_REDIR_ALLOCATION "redirection allocation failed"
# define E_REDIR_INVALID "syntax error near unexpected token"
# define E_SEMICOLON "syntax error near unexpected token ';'"
# define E_UNCLOSED_DBL_QUOTE "unexpected EOF while looking for matching `\"'"
# define E_UNCLOSED_SGL_QUOTE "unexpected EOF while looking for matching `''"
# define E_UNSET_START "unset: "
# define E_UNSET_END ": invalid option"

/* execution error messages */
# define E_CD_NAME_TOO_LONG "cd: file name too long: "
# define E_CD_NOT_DIR "cd: not a directory: "
# define E_CD_NO_SUCH "cd: no such file or directory: "
# define E_CD_PERMISSION "cd: permission denied: "
# define E_CD_TOO_MANY "cd: too many arguments"
# define E_CMD_NOT_FOUND "command not found: "
# define E_CMD_NOT_FOUND_EMPTY "command not found: (empty command)"
# define E_EXIT_ARG "exit: "
# define E_EXIT_NUMERIC ": numeric argument required"
# define E_EXIT_TOO_MANY "exit: too many arguments"
# define E_IS_DIRECTORY "is a directory: "
# define E_PERMISSION_DENIED "permission denied: "
# define E_PERMISSION_DENIED_EMPTY "permission denied: (empty command)"
# define E_PWD_NOT_FOUND "PWD not found"

/* allocation and initialization error messages */
# define E_ADD_LINE_INVALID "add line to string: invalid line"
# define E_ALLOCATE_NEW_STR "allocate_new_str: allocation failed"
# define E_CATCH_ABSENT_VAR "allocation error on catch_absent_var"
# define E_CATCH_VAR "catch_var: allocation error"
# define E_INIT_VAR_ARRAYS "init_var_expand_arrays: allocation failed"
# define E_INIT_VAR_VALUES "init_var_expand_arrays allocation failed on values"
# define E_ITOA_FAILED "ft_itoa failed for $? expansion"

/* AST and parsing error messages */
# define E_AST_ROOT_NULL "exec_ast_root: on execution, ast node is NULL"
# define E_AST_ROOT_SIG "exec_ast_root: failed to set execution signal handling"
# define E_EXEC_AST_NULL "exec_ast: ast root node is NULL"
# define E_FAILED_ALLOC_AST "failed to allocate AST nodes"
# define E_HEREDOC_AST_NULL "heredoc_ast_node, ast node is NULL"
# define E_LOOKUP_AST_NULL "lookup_all_cmd_fullpaths: ast node is NULL"

/* heredoc error messages */
# define E_HEREDOC_REDIR "heredoc redir failed"
# define E_HEREDOC_SIG "failed to set heredoc signal handler"

/* signal handling error messages */
# define E_SIGNAL_INTERACTIVE "set interactive signal handling failed"

/* system operation error messages (for ms_perror) */
# define E_ADD_CHUNK "add chunk"
# define E_ADD_LINE_STRING "add line to string"
# define E_ALLOC "alloc"
# define E_DUP2 "dup2"
# define E_FORK_CMD "cmd node fork"
# define E_FORK_PIPE "pipe node fork"
# define E_INIT_REMOVE_QUOTES "init_remove_quotes"
# define E_OPEN_HEREDOC "open hdoc"
# define E_PIPE "pipe"
# define E_REDIR_INPUT_FAILED "error: failed to redirect input"
# define E_REDIR_OUTPUT_FAILED "error: failed to redirect output"
# define E_WAIT "wait error"
# define E_WRITE "write"

/* heredoc warning messages */
# define E_HDOC_EOF_START "warning: here-document delimited by end-of-file \
(wanted `"
# define E_HDOC_EOF_END "')"
#endif
