/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:29:39 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/05 11:04:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *s, int len)
{
	char	*token;
	int		i;

	i = 0;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	token[len] = '\0';
	while (i < len)
	{
		token[i] = s[i];
		i++;
	}
	return (token);
}
