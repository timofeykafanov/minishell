/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:23:40 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/19 16:24:14 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	size_t	total_size;
	char	*ptr;

	if (nmemb && size > SIZE_MAX / nmemb)
		return (NULL);
	total_size = nmemb * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	if (total_size > 0)
	{
		i = 0;
		while (i < total_size)
		{
			ptr[i] = 0;
			i++;
		}
	}
	return (ptr);
}
