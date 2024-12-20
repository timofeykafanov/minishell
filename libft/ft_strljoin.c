/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strljoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:21:03 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/19 17:19:24 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strljoin(char *s1, char *s2, int len)
{
	char	*res;
	int		i;
	int		j;

	if (!s1)
		return (ft_strndup(s2, len));
	res = malloc(sizeof(char) * (ft_strlen(s1) + len + 1));
	if (!res)
		return (free(s1), NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	while (j < len)
	{
		res[i] = s2[j];
		i++;
		j++;
	}
	res[i] = '\0';
	free(s1);
	return (res);
}
