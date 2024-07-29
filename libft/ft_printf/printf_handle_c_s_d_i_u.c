/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_handle_c_s_d_i_u.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:22:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/06/27 16:08:40 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	handle_c(va_list type, int *count, int fd)
{
	char	c;

	c = va_arg(type, int);
	printf_ft_putchar(c, count, fd);
}

void	handle_s(va_list type, int *count, int fd)
{
	char	*str;

	str = va_arg(type, char *);
	if (!str)
		printf_ft_putstr("(null)", count, fd);
	else
		printf_ft_putstr(str, count, fd);
}

void	handle_d_and_i(va_list type, int *count, int fd)
{
	char	*str;

	str = ft_itoa(va_arg(type, int));
	if (!str)
		return ;
	printf_ft_putstr(str, count, fd);
	free(str);
}

void	handle_u(va_list type, int *count, int fd)
{
	char	*str;

	str = printf_ft_itoa_u(va_arg(type, unsigned int));
	if (!str)
		return ;
	printf_ft_putstr(str, count, fd);
	free(str);
}
