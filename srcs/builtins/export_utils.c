/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:20:11 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 11:18:30 by tkafanov         ###   ########.fr       */
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

static int	is_special_char(char c)
{
	if (c == '\\' || c == '$' || c == '\"' || c == '`')
		return (1);
	return (0);
}

static void	check_equal_sign(char **copy, int i)
{
	int		j;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

	equal_sign = ft_strchr(copy[i], '=');
	if (equal_sign)
	{
		j = 0;
		var_name = ft_strndup(copy[i], equal_sign - copy[i]);
		var_value = ft_strdup(equal_sign + 1);
		printf("declare -x %s=\"", var_name);
		while (var_value[j])
		{
			if (is_special_char(var_value[j]))
				printf("\\");
			printf("%c", var_value[j]);
			j++;
		}
		printf("\"\n");
		free(var_name);
		free(var_value);
	}
	else
		printf("declare -x %s\n", copy[i]);
}

static void	print_vars(char	**copy)
{
	int		i;

	i = 0;
	while (copy[i])
	{
		check_equal_sign(copy, i);
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
		exit(ERROR);
	while (memory->env[i])
	{
		copy[i] = ft_strdup(memory->env[i]);
		i++;
	}
	copy[i] = NULL;
	bubble_sort_char_array(copy);
	print_vars(copy);
	free_env(copy);
}
