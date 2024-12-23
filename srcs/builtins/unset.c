/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:09:53 by sopperma          #+#    #+#             */
/*   Updated: 2024/12/23 21:08:39 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	remove_var(t_memory *memory, char **args, int i)
{
	int	j;

	j = 0;
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
}

void	unset(t_memory *memory, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "OLDPWD") == 0)
		{
			if (memory->oldpwd)
			{
				free(memory->oldpwd);
				memory->oldpwd = NULL;
			}
		}
		remove_var(memory, args, i);
		i++;
	}
}
