/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/16 19:00:28 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env(t_memory *memory)
{
	int	i;

	i = 0;
	while (memory->env[i])
	{
		if (ft_strchr(memory->env[i], '='))
			ft_printf("%s\n", STDOUT_FILENO, memory->env[i]);
		i++;
	}
}

void	unset(t_memory *memory, char **args)
{
	int	i;
	int	j;

	i = 1;
	while (args[i])
	{
		j = 0;
		if (ft_strcmp(args[i], "OLDPWD") == 0)
		{
			if (memory->oldpwd)
			{
				free(memory->oldpwd);
				memory->oldpwd = NULL;
			}
		}
		while (memory->env[j] && j < memory->env_lines)
		{
			if (ft_strncmp(memory->env[j], args[i], ft_strlen(args[i])) == 0)
			{
				free(memory->env[j]);
				memory->env_lines--;
				while (memory->env[j + 1])
				{
					memory->env[j] = memory->env[j + 1];
					j++;
				}
				memory->env[j] = NULL;
				break ;
			}
			j++;
		}
		i++;
	}
}
