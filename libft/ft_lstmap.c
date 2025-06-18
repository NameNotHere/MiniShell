/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otanovic <otanovic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:11:38 by otanovic          #+#    #+#             */
/*   Updated: 2024/12/01 12:10:01 by otanovic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*nlst;
	t_list	*new_node;
	void	*content;

	if (!f || !lst || !del)
		return (NULL);
	nlst = NULL;
	while (lst != NULL)
	{
		content = f(lst->content);
		if (!content)
			return (ft_lstclear(&nlst, del), NULL);
		new_node = ft_lstnew(content);
		if (!new_node)
			return (ft_lstclear(&nlst, del), del(content), NULL);
		ft_lstadd_back(&nlst, new_node);
		lst = lst->next;
	}
	return (nlst);
}
