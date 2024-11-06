/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:41:43 by sopperma          #+#    #+#             */
/*   Updated: 2024/11/06 11:18:13 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_args(char **args, int i)
{
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
}

void	echo(char **args)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		printf("\n");
		return ;
	}
	if (ft_strncmp(args[i], "-n", 2) == 0)
	{
		i++;
		print_args(args, i);
	}
	else
	{
		print_args(args, i);
		printf("\n");
	}
}
