/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 11:28:31 by tkafanov          #+#    #+#             */
/*   Updated: 2024/05/21 09:13:55 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;
	t_list	*ptr_res;
	void	*temp;

	if (!lst || !f || !del)
		return (NULL);
	temp = f(lst->content);
	res = ft_lstnew(temp);
	if (!res)
		return (free(temp), NULL);
	ptr_res = res;
	lst = lst->next;
	while (lst)
	{
		temp = f(lst->content);
		ptr_res->next = ft_lstnew(temp);
		if (!ptr_res->next)
		{
			ft_lstclear(&res, del);
			return (free(temp), NULL);
		}
		ptr_res = ptr_res->next;
		lst = lst->next;
	}
	return (res);
}
