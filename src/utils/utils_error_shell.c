/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:44:00 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/25 16:47:05 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_stderr_code(t_msh *sh, const char *error, int exit_code)
{
	put_stderr(error);
	sh->exit_code = exit_code;
}
