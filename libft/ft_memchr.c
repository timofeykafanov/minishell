/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:22:53 by tkafanov          #+#    #+#             */
/*   Updated: 2024/05/21 09:14:08 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*p;
	unsigned char		value;

	i = 0;
	p = (unsigned char *)s;
	value = (unsigned char)c;
	while (i < n)
	{
		if (p[i] == value)
			return ((void *)&p[i]);
		i++;
	}
	return (0);
}
