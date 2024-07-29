/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_handle_p_x_percent.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 14:28:23 by tkafanov          #+#    #+#             */
/*   Updated: 2024/06/27 16:08:38 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	put_hex(unsigned long num, char chr, int *count, int fd)
{
	if (num >= 16)
	{
		put_hex(num / 16, chr, count, fd);
		put_hex(num % 16, chr, count, fd);
	}
	else
	{
		if (num <= 9)
			printf_ft_putchar(num + '0', count, fd);
		else
		{
			if (chr == 'X')
				printf_ft_putchar(num - 10 + 'A', count, fd);
			else
				printf_ft_putchar(num - 10 + 'a', count, fd);
		}
	}
}

void	handle_p(va_list type, int *count, int fd, char chr)
{
	unsigned long	num;

	num = va_arg(type, unsigned long);
	if (!num)
		printf_ft_putstr("(nil)", count, fd);
	else
	{
		printf_ft_putstr("0x", count, fd);
		put_hex(num, chr, count, fd);
	}
}

void	handle_x(va_list type, int *count, int fd, char chr)
{
	int long	num;

	num = va_arg(type, unsigned int);
	put_hex(num, chr, count, fd);
}

void	handle_percent(int *count, int fd)
{
	printf_ft_putchar('%', count, fd);
}
