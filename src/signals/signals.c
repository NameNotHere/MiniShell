/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 20:33:25 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/20 20:43:08 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_signal.h"

/*
	g_sig, t_sa and t_handler:
		- explained in minishell_signal header
*/

volatile sig_atomic_t	g_sig = 0;