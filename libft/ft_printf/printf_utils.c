/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:56:43 by tkafanov          #+#    #+#             */
/*   Updated: 2024/06/28 15:20:03 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

size_t	printf_ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	printf_ft_putchar(char c, int *count, int fd)
{
	*count += write(fd, &c, 1);
}

void	printf_ft_putstr(char *str, int *count, int fd)
{
	while (*str)
		*count += write(fd, &(*str++), 1);
}

char	*printf_ft_strdup(const char *s)
{
	char	*dup_str;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(s);
	dup_str = (char *)malloc((len + 1) * sizeof(char));
	if (!dup_str)
		return (NULL);
	while (s[i])
	{
		dup_str[i] = s[i];
		i++;
	}
	dup_str[i] = '\0';
	return (dup_str);
}
