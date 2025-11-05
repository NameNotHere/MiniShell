/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_readline_state.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 02:33:08 by tda-roch          #+#    #+#             */
/*   Updated: 2025/11/02 20:30:09 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
	Updates line made, adding a new chunk read.
		- uses rln_state to get/set lenghts/positions.
	Returns:
		true: if chunk added
		false: if chunk adding failed
*/
bool	add_chunk(t_rln_state *st, const char *src, size_t n)
{
	char	*new_line_made;
	size_t	new_len;

	if (n == 0)
		return (true);
	new_len = st->made_len + n;
	new_line_made = ft_calloc(new_len + 1, sizeof(char));
	if (!new_line_made)
	{
		ms_perror(E_ADD_CHUNK);
		return (false);
	}
	if (st->made_len > 0 && st->line_made)
		ft_memcpy(new_line_made, st->line_made, st->made_len);
	ft_memcpy(new_line_made + st->made_len, src, n);
	new_line_made[new_len] = '\0';
	free(st->line_made);
	st->line_made = new_line_made;
	st->made_len = new_len;
	return (true);
}

/*
	Set line to partial line built so far, or NULL if nothing available.
	Return: true
*/
bool	rln_flush_line(t_rln_state *st, char **line)
{
	if (!(st->line_made && st->made_len > 0))
	{
		*line = NULL;
		return (true);
	}
	st->line_made[st->made_len] = '\0';
	*line = st->line_made;
	st->line_made = NULL;
	st->made_len = 0;
	return (true);
}

/*
	Initialize line read state and return true
	If null read buffer or line pointer: return false
*/
bool	rln_init(t_rln_state *st, t_readbuf *rb, char **line)
{
	if (!rb || !line)
		return (false);
	st->line_made = NULL;
	st->made_len = 0;
	st->end = 0;
	*line = NULL;
	return (true);
}

/*
	Newline found: advance read buffer position past it.
	Set line to the accumulated line_made.
*/
bool	rln_emit_line(t_rln_state *st, t_readbuf *rb, char **line)
{
	rb->pos = st->end + 1;
	if (!st->line_made)
	{
		*line = ft_calloc(1, sizeof(char));
		return (*line != NULL);
	}
	*line = st->line_made;
	return (true);
}
