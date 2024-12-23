/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:41:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/23 21:33:23 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

static void	print_args(char **args, int i)
{
	while (args[i])
	{
		ft_printf("%s", STDOUT_FILENO, args[i]);
		if (args[i + 1])
			ft_printf(" ", STDOUT_FILENO);
		i++;
	}
}

static bool	is_only_n_flags(char *arg)
{
	int	i;

	i = 1;
	if (arg[0] != '-')
		return (false);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	if (i == 1)
		return (false);
	return (true);
}

void	echo(char **args)
{
	int		i;
	bool	n_flag;

	n_flag = false;
	i = 1;
	if (!args[1])
	{
		ft_printf("\n", STDOUT_FILENO);
		return ;
	}
	while (args[i] && is_only_n_flags(args[i]))
	{
		i++;
		n_flag = true;
	}
	if (n_flag)
		print_args(args, i);
	else
	{
		print_args(args, i);
		ft_printf("\n", STDOUT_FILENO);
	}
}
