/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:41:19 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/05 15:30:58 by tkafanov         ###   ########.fr       */
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

static int is_special_char(char c)
{
	if (c == '\\' || c == '$' || c == '\"'  || c == '`')
		return (1);
	return (0);
}

static void	print_export(t_memory *memory)
{
	int		i;
	int 	j;
	char	**copy;
	char	*equal_sign;
	char	*var_name;
	char	*var_value;

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
	i = 0;
	while (copy[i])
	{
		equal_sign = ft_strchr(copy[i], '=');
		if (equal_sign)
		{
			j = 0;
			var_name = ft_strndup(copy[i], equal_sign - copy[i]);
			var_value = ft_strdup(equal_sign + 1);
			printf("declare -x %s=\"", var_name);
			while(var_value[j])
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
		{
			printf("declare -x %s\n", copy[i]);
		}
		i++;
	}
	free_env(copy);
}

static void	add_env_var(t_memory *memory, char **args)
{
	int	i;
	int	j;
	int found;
	char *var_name;

	j = 1;
	found = 0;
	while (args[j])
	{
		i = 0;
		found = 0;
		if (ft_strchr(args[j], '='))
			var_name = ft_strncpy(args[j], ft_strchr(args[j], '=') - args[j]);
		else
			var_name = ft_strdup(args[j]);
		while (memory->env[i] && i < memory->env_lines)
		{
			if(memory->env[i] && ft_strncmp(memory->env[i], var_name, ft_strlen(var_name)) == 0)
			{
				printf("changed: %s on line %d to %s\n", memory->env[i], i, args[j]);
				free(memory->env[i]);
				memory->env[i] = ft_strdup(args[j]);
				found = 1;
				break;
			}
			i++;
		}
		if (found)
		{
			j++;
			continue;
		}
		memory->env[memory->env_lines] = ft_strdup(args[j]);
		printf("added: %s on line %d\n", memory->env[memory->env_lines], memory->env_lines);
		printf("env_lines: %d\n", memory->env_lines);
		memory->env_lines++;
		if (memory->env_lines == memory->env_space)
		{
			memory->env = ft_realloc(memory->env, \
				sizeof(char *) * (memory->env_lines + 512 + 1));
			memory->env_space += 512;
		}
		memory->env[memory->env_lines] = NULL;
		free(var_name);
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
