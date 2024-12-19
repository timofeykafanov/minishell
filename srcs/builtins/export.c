/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:41:19 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/19 16:56:25 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	find_var(t_memory *memory, char **args, int j, int *found)
{
	int		i;
	char	*var_name;
	size_t	env_var_len;

	i = 0;
	if (ft_strchr(args[j], '='))
		var_name = ft_strncpy(args[j], \
		ft_strchr(args[j], '=') - args[j]);
	else
		var_name = ft_strdup(args[j]);
	if (!var_name)
	{
		free_memory(memory);
		exit(ERROR);
	}
	while (memory->env[i] && i < memory->env_lines)
	{
		env_var_len = ft_strchr(memory->env[i], '=') - memory->env[i];
		if (memory->env[i] && ft_strncmp(memory->env[i], var_name, \
			ft_strlen(var_name)) == 0 && env_var_len == ft_strlen(var_name))
		{
			free(memory->env[i]);
			memory->env[i] = ft_strdup(args[j]);
			if (!memory->env[i])
			{	
				free(var_name);
				free_memory(memory);
				exit(ERROR);
			}
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
	while (args[j])
	{
		found = 0;
		find_var(memory, args, j, &found);
		if (found)
		{
			j++;
			continue ;
		}
		memory->env[memory->env_lines] = ft_strdup(args[j]);
		if (!memory->env[memory->env_lines])
		{
			free_memory(memory);
			exit(ERROR);
		}
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
