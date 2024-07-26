/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 09:22:33 by tkafanov          #+#    #+#             */
/*   Updated: 2024/05/21 09:16:37 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*match;

	match = 0;
	while (*s)
	{
		if (*s == (char)c)
			match = (char *)s;
		s++;
	}
	if (*s == (char)c)
		match = (char *)s;
	return (match);
}
