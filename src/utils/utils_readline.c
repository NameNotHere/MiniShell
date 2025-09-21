/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_readline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tda-roch <tda-roch@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:07:24 by tda-roch          #+#    #+#             */
/*   Updated: 2025/09/21 03:02:06 by tda-roch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	If newline char found, returns offset from buffer all the way to the nl char
	found (calculated using pointer arithmetic);
	If no nl char found, returns full lenght of the buffer so far (end)
*/
static inline ssize_t	rln_find_nl_or_end(const t_readbuf *rb)
{
	const char	*nl;

	nl = ft_memchr(rb->buf + rb->pos, '\n', (size_t)(rb->len - rb->pos));
	if (nl)
		return ((ssize_t)(nl - rb->buf));
	return (rb->len);
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
				return (rln_flush_line(&st, line));
			if (rb->len < 0 && (errno == EINTR))
				continue ;
			if (rb->len < 0)
				return (safe_free_string(&st.line_made), false);
		}
		st.end = rln_find_nl_or_end(rb);
		if ((st.end > rb->pos)
			&& !add_chunk(&st, rb->buf + rb->pos, (size_t)(st.end - rb->pos)))
			return (safe_free_string(&st.line_made), false);
		if (st.end < rb->len && rb->buf[st.end] == '\n')
			return (rln_emit_line(&st, rb, line));
		rb->pos = st.end;
	}
}
