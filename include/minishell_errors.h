/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_errors.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:56:51 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/10 17:01:53 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_ERRORS_H
# define MINISHELL_ERRORS_H


/* minishell start error message */
# define E_MINISHELL "minishell: "

/* minishell initialization and syntax error messages */
# define E_CD_CWD_NULL "cd: error retrieving current directory: getcwd: cannot \
access parent directories: No such file or directory"
# define E_EXPORT_END "': not a valid identifier"
# define E_EXPORT_START "export: `"
# define E_OPTION_C_ARGUMENT "-c: option requires an argument"
# define E_REDIR_INVALID "syntax error near unexpected token"
# define E_SEMICOLON "syntax error near unexpected token ';'"
# define E_UNCLOSED_DBL_QUOTE "unexpected EOF while looking for matching `\"'"
# define E_UNCLOSED_SGL_QUOTE "unexpected EOF while looking for matching `''"
# define E_UNSET_START "unset: "
# define E_UNSET_END ": invalid option"

/* execution error messages */
# define E_ENV_ARGS_NOT_SUPPORTED "env: arguments not supported"
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
# define E_PERM_DENIED "permission denied: "
# define E_PERM_DENIED_EMPTY "permission denied: (empty command)"
# define E_PWD_NOT_FOUND "PWD not found"

/* allocation and initialization error messages */
# define E_ALLOC_CD "cd: memory allocation"
# define E_ALLOC_NEW_STR "var expansion: new string: memory allocation"
# define E_ALLOC_REDIR "add redir: allocation"
# define E_ALLOC_AST_NODE "make AST node: allocation"
# define E_ALLOC_RLN_CHUNK "readline non-interact: add chunk"
# define E_CATCH_ABSENT_VAR "catch absent var: allocation error"
# define E_CATCH_VAR "catch var: allocation error"

# define E_INIT_ENV "init_var_expand_arrays: allocation failed"
# define E_INIT_VAR_ARRAYS "init_var_expand_arrays: allocation failed"
# define E_INIT_VAR_VALUES "init_var_expand_arrays allocation failed on values"
# define E_ADD_LINE_INVALID "add line to string: invalid line"
# define E_ITOA_FAILED "ft_itoa failed for $? expansion"

/* AST and parsing error messages */
# define E_AST_BUILD_FAILED "parse line: AST build failed"
# define E_AST_ROOT_NULL "exec AST root: on execution, AST node is NULL"
# define E_SYNTAX_AMPERSAND "syntax error near unexpected token `&'"
# define E_SYNTAX_AND "syntax error near unexpected token `&&'"
# define E_SYNTAX_OR "syntax error near unexpected token `||'"
# define E_SYNTAX_PIPE "syntax error near unexpected token `|'"
# define E_SYNTAX_LPAREN "syntax error near unexpected token `('"
# define E_SYNTAX_RPAREN "syntax error near unexpected token `)'"
# define E_AST_ROOT_SIG "AST root: failed to set exec signal handling"
# define E_EXEC_AST_NULL "exec AST: AST root node is NULL"
# define E_HEREDOC_AST_NULL "heredoc AST node: AST node is NULL"
# define E_LOOKUP_AST_NULL "lookup all cmd fullpaths: AST node is NULL"
# define E_TOKENIZE_FAILED "parse line: tokenizer failed"

/* heredoc error messages */
# define E_HEREDOC_REDIR "heredoc redir failed"
# define E_HEREDOC_SIG "failed to set heredoc signal handler"

/* builtin execution error messages */
# define E_BUILTIN_REDIR_FAILED "exec_single_builtin: failed to save \
stdin/stdout for builtin redirection"
# define E_GET_ENV_VALUE "get_env_value allocation"

/* signal handling error messages */
# define E_SIGNAL_INTERACTIVE "set interactive signal handling failed"

/* system operation error messages (for msg_perr) */
# define E_ADD_LINE_STRING "add line to string"
# define E_DUP2 "dup2"
# define E_FORK_CMD "cmd node fork"
# define E_FORK_PIPE "pipe node fork"
# define E_INIT_REMOVE_QUOTES "init_remove_quotes"
# define E_OPEN_HEREDOC "open: hdoc"
# define E_OPEN_RUN_SCRIPT "open: run script fd"
# define E_PIPE "pipe"
# define E_REDIR_INPUT_FAILED "failed to redirect input"
# define E_REDIR_OUTPUT_FAILED "failed to redirect output"
# define E_WAIT "wait"
# define E_WRITE "write"
# define E_ALLOC "allocation"

/* heredoc warning messages */
# define E_HDOC_EOF_START "warning: here-document delimited by end-of-file \
(wanted `"
# define E_HDOC_EOF_END "')"

#endif
