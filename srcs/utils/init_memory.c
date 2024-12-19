/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:56:30 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/19 14:17:57 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_env_var(t_memory *memory, char *var)
{
	int	i;

	i = 0;
	while (memory->env[i])
	{
		if (ft_strncmp(memory->env[i], var, ft_strlen(var)) == 0)
			return (memory->env[i] + ft_strlen(var));
		i++;
	}
	return (NULL);
}

static void	create_env(t_memory *memory, char **env)
{
	int	i;

	i = 0;
	memory->env = malloc(sizeof(char *) * 512);
	if (!memory->env)
		return ;
	memory->env_lines = 0;
	memory->env_space = 512;
	while (env[i])
	{
		memory->env[i] = ft_strdup(env[i]);
		memory->env_lines++;
		i++;
	}
	memory->env[i] = NULL;
}

t_memory	*init_memory(char **env)
{
	t_memory	*memory;

	memory = malloc(sizeof(*memory));
	if (!memory)
		return (NULL);
	ft_bzero(memory, sizeof(*memory));
	create_env(memory, env);
	if (!memory->env)
		return (free_memory(memory), NULL);
	memory->pwd = malloc(PATH_MAX);
	if (!memory->pwd)
		return (free_memory(memory), NULL);
	getcwd(memory->pwd, PATH_MAX);
	memory->oldpwd = ft_strdup(memory->pwd);
	return (memory);
}
