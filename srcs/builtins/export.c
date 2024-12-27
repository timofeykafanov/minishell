/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 15:41:19 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:05:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	put_var(t_memory *memory, char **args, int j)
{
	memory->env[memory->env_lines] = ft_strdup(args[j]);
	if (!memory->env[memory->env_lines])
		end_shell(memory);
	memory->env_lines++;
	if (memory->env_lines == memory->env_space)
	{
		memory->env = ft_realloc(memory->env, \
			sizeof(char *) * (memory->env_lines + 512 + 1));
		if (!memory->env)
			end_shell(memory);
		memory->env_space += 512;
	}
	memory->env[memory->env_lines] = NULL;
}

static void	replace(t_memory *memory, char *arg, char *var_name)
{
	int		i;
	size_t	env_var_len;

	i = 0;
	while (memory->env[i] && i < memory->env_lines)
	{
		env_var_len = ft_strchr(memory->env[i], '=') - memory->env[i];
		if (!ft_strchr(memory->env[i], '='))
			env_var_len = ft_strlen(memory->env[i]);
		if (memory->env[i] && ft_strncmp(memory->env[i], var_name, \
			ft_strlen(var_name)) == 0 && env_var_len == ft_strlen(var_name))
		{
			free(memory->env[i]);
			memory->env[i] = ft_strdup(arg);
			if (!memory->env[i])
			{
				free(var_name);
				end_shell(memory);
			}
			memory->found = true;
			break ;
		}
		i++;
	}
}

static void	find_var(t_memory *memory, char *arg)
{
	char	*var_name;
	char	*has_value;

	if (ft_strchr(arg, '='))
		var_name = ft_strncpy(arg, ft_strchr(arg, '=') - arg);
	else
		var_name = ft_strdup(arg);
	if (!var_name)
	{
		free_memory(memory);
		exit(ERROR);
	}
	has_value = ft_strchr(arg, '=');
	if (has_value)
		replace(memory, arg, var_name);
	else if (find_env_index(memory->env, var_name) != -1)
		memory->found = true;
	else
		memory->found = false;
	free(var_name);
}

static void	add_env_var(t_memory *memory, char **args)
{
	int		j;

	j = 1;
	while (args[j])
	{
		memory->found = false;
		find_var(memory, args[j]);
		if (memory->found)
		{
			j++;
			memory->found = false;
			continue ;
		}
		put_var(memory, args, j);
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
