/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 22:09:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/24 12:45:54 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	contains_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && \
			str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
			return (false);
		i++;
	}
	return (true);
}

bool	contains_only_digits(char *str, int *sign)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || \
		str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			if (contains_only_spaces(str + i))
				return (true);
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	check_long_max(char *str)
{
	int			i;
	static char	*max_str = "9223372036854775807";

	i = 0;
	while (i < 19)
	{
		if (str[i] > max_str[i])
			return (false);
		if (str[i] < max_str[i])
			break ;
		i++;
	}
	return (true);
}

static bool	check_long_min(char *str)
{
	static char	*min_str = "9223372036854775808";
	int			i;

	i = 0;
	while (i < 19)
	{
		if (str[i] > min_str[i])
			return (false);
		if (str[i] < min_str[i])
			break ;
		i++;
	}
	return (true);
}

bool	check_overflow(int sign, char *str)
{
	if (sign == 1)
		return (check_long_max(str));
	else
		return (check_long_min(str));
	return (true);
}
