/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:41:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/27 17:28:03 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_args(char **args, int i)
{
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

static bool	is_only_n_flags(char *arg)
{
	int	i;

	i = 1;
	// if (!arg)
	// 	return (false);
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
		printf("\n");
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
		printf("\n");
	}
}
