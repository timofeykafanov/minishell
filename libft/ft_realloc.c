/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:19:36 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/29 13:22:32 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void *new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

// void	*ft_realloc(void *ptr, size_t size)
// {
//	void *res;

//	res = malloc(size);
//	if (!res)
//		return (NULL);
//	ft_memcpy(res, ptr, size);
//	free(ptr);
//	return (res);
// }