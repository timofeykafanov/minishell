/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:21:50 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 12:22:10 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char    *ft_strndup(char *s, int len)
{
    char *res;
    int i;

    res = malloc(sizeof(char) * (len + 1));
    if (!res)
        return (NULL);
    i = 0;
    while (i < len)
    {
        res[i] = s[i];
        i++;
    }
    res[i] = '\0';
    return (res);
}