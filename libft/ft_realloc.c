/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 12:19:36 by sopperma          #+#    #+#             */
/*   Updated: 2024/08/08 12:19:45 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void *ft_realloc(void *ptr, size_t size)
{
    void *res;

    res = malloc(size);
    if (!res)
        return (NULL);
    ft_memcpy(res, ptr, size);
    free(ptr);
    return (res);
}