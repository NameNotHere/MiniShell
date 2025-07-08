/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:27:17 by tda-roch          #+#    #+#             */
/*   Updated: 2025/07/08 17:11:00 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
This just prints a debug print to check redir received at execution
TODO: REMOVE THIS FUNCTION BEFORE EVAL
*/
void	debug_print_one_redir(t_redir *redir)
{
	if (redir && redir->string)
	{
		d_print("redir type: %s string: |%s|\n",
			get_redir_symbol(redir->ty),
			redir->string);
		debug_print_one_redir(redir->next);
	}
}
