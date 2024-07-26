/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:21:15 by tkafanov          #+#    #+#             */
/*   Updated: 2024/05/21 09:14:18 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*pdest;
	const unsigned char	*psrc;
	size_t				i;

	if (!dest && !src && n != 0)
		return (dest);
	pdest = dest;
	psrc = src;
	if (pdest < psrc)
	{
		i = -1;
		while (++i < n)
			pdest[i] = psrc[i];
	}
	else
	{
		i = n;
		while (i != 0)
		{
			i--;
			pdest[i] = psrc[i];
		}
	}
	return (dest);
}
