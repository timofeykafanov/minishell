/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:20:11 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:21:20 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	print_export(t_memory *memory)
{
	int		i;
	char	**copy;

	i = 0;
	copy = malloc(sizeof(char *) * (memory->env_lines + 1));
	if (!copy)
		end_shell(memory);
	while (memory->env[i])
	{
		copy[i] = ft_strdup(memory->env[i]);
		if (!copy[i])
		{
			free_env(copy);
			end_shell(memory);
		}
		i++;
	}
	copy[i] = NULL;
	bubble_sort_char_array(copy);
	print_vars(memory, copy);
	free_env(copy);
}
