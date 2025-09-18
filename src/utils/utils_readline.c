/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_readline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:07:24 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/18 09:15:39 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


bool	add_part(t_rln_state *st, const char *src, size_t n)
{
	char	*new_line_made;
	size_t	new_len;

	if (n == 0)
		return (true);
	new_len = st->made_len + n;
	new_line_made = ft_calloc(new_len + 1, sizeof(char));
	if (!new_line_made)
	{
		perror("add part");
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
static inline bool	flush_line_build(t_rln_state *st, char **line)
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
static inline bool	rln_init(t_rln_state *st, t_readbuf *rb, char **line)
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
	If newline char found, returns offset from buffer all the way to the nl char
	found (calculated using pointer arithmetic);
	If no nl char found, returns full lenght of the buffer so far (end)
*/
static inline ssize_t	rbuf_find_nl_or_end(const t_readbuf *rb)
{
	const char	*nl;

	nl = ft_memchr(rb->buf + rb->pos, '\n', (size_t)(rb->len - rb->pos));
	if (nl)
		return ((ssize_t)(nl - rb->buf));
	return (rb->len);
}


/*
	Newline found: advance read buffer position past it.
	Set line to the accumulated line_build.
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

/*
Non-interactive line reader using read().
- Parameters: fd to read from; rb is the read_buffer to be used, line is
	pointer to line to be extracted from read.
- Returns:
	- true on success (line set or NULL on EOF)
	- false on error (errno set).
- Uses a caller-provided persistent buffer (rb) to keep leftovers between calls,
  so data after a newline is preserved for the next invocation.

Notes:
- This implementation intentionally avoids getline().
- Lines are returned without the trailing newline. Empty lines get "".
*/
bool	readline_noninteract(int fd, t_readbuf *rb, char **line)
{
	t_rln_state	st;

	if (rln_init(&st, rb, line) == false)
		return (false);
	while (true)
	{
		if (rb->pos >= rb->len)
		{
			rb->len = read(fd, rb->buf, sizeof(rb->buf));
			rb->pos = 0;
			if (rb->len == 0)
				return (flush_line_build(&st, line));
			if (rb->len < 0 && (errno == EINTR))
				continue ;
			if (rb->len < 0)
				return (safe_free_string(&st.line_made), false);
		}
		st.end = rbuf_find_nl_or_end(rb);
		if ((st.end > rb->pos)
			&& !add_part(&st, rb->buf + rb->pos, (size_t)(st.end - rb->pos)))
			return (safe_free_string(&st.line_made), false);
		if (st.end < rb->len && rb->buf[st.end] == '\n')
			return (rln_emit_line(&st, rb, line));
		rb->pos = st.end;
	}
}

