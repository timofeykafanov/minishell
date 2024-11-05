/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:41:19 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/05 13:55:25 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	find_var(t_memory *memory, char **args, int j, int *found)
{
	int		i;
	char	*var_name;

	i = 0;
	*found = 0;
	if (ft_strchr(args[j], '='))
		var_name = ft_strncpy(args[j], \
		ft_strchr(args[j], '=') - args[j]);
	else
		var_name = ft_strdup(args[j]);
	while (memory->env[i] && i < memory->env_lines)
	{
		if (memory->env[i] && ft_strncmp(memory->env[i], var_name, \
			ft_strlen(var_name)) == 0)
		{
			free(memory->env[i]);
			memory->env[i] = ft_strdup(args[j]);
			*found = 1;
			break ;
		}
		i++;
	}
	free(var_name);
}

static void	add_env_var(t_memory *memory, char **args)
{
	int	j;
	int	found;

	j = 1;
	found = 0;
	while (args[j])
	{
		find_var(memory, args, j, &found);
		if (found)
		{
			j++;
			continue ;
		}
		memory->env[memory->env_lines] = ft_strdup(args[j]);
		memory->env_lines++;
		if (memory->env_lines == memory->env_space)
		{
			memory->env = ft_realloc(memory->env, \
				sizeof(char *) * (memory->env_lines + 512 + 1));
			memory->env_space += 512;
		}
		memory->env[memory->env_lines] = NULL;
		j++;
	}
}

void	execute_export(t_memory *memory, char **args)
{
	if (!args[1])
		print_export(memory);
	else
		add_env_var(memory, args);
}
