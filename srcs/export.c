/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:41:19 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 16:02:20 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	bubble_sort_char_array(char **array)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[j])
		{
			if (ft_strncmp(array[i], array[j], ft_strlen(array[i])) < 0)
			{
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
			j++;
		}
		i++;
	}
}

static int	print_export(t_memory *memory)
{
	int		i;
	char	**copy;

	i = 0;
	copy = malloc(sizeof(char *) * (memory->env_lines + 1));
	if (!copy)
		return (ERROR);
	while (memory->env[i])
	{
		copy[i] = ft_strdup(memory->env[i]);
		i++;
	}
	copy[i] = NULL;
	bubble_sort_char_array(copy);
	i = 0;
	while (copy[i])
		printf("declare -x %s\n", copy[i++]);
	free_env(copy);
	return (1);
}

static void	add_env_var(t_memory *memory, char **args)
{
	int	i;
	int	j;

	j = 1;
	i = 0;
	while (memory->env[i] && i < memory->env_lines)
		i++;
	while (args[j])
	{
		if (memory->env_lines == memory->env_space)
		{
			memory->env = ft_realloc(memory->env, \
				sizeof(char *) * (memory->env_lines + 512 + 1));
			memory->env_space += 512;
		}
		memory->env[i++] = ft_strdup(args[j++]);
		memory->env_lines++;
	}
	memory->env[i] = NULL;
}

void	execute_export(t_memory *memory, char **args)
{
	if (!args[1])
		print_export(memory);
	else
		add_env_var(memory, args);
}
