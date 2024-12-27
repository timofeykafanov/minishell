/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:56:30 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:10:03 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (!memory->env[i])
			end_shell(memory);
		memory->env_lines++;
		i++;
	}
	memory->env[i] = NULL;
}

static void	generate_env(t_memory *memory)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	memory->env = malloc(sizeof(char *) * 512);
	if (!memory->env)
		end_shell(memory);
	memory->env_lines = 3;
	memory->env_space = 512;
	if (!memory->env)
		end_shell(memory);
	memory->env[0] = ft_strdup("OLDPWD");
	if (!memory->env[0])
		end_shell(memory);
	memory->env[1] = ft_strjoin("PWD=", pwd);
	if (!memory->env[1])
		end_shell(memory);
	memory->env[2] = ft_strdup("SHLVL=1");
	if (!memory->env[2])
		end_shell(memory);
	memory->env[3] = NULL;
}

t_memory	*init_memory(char **env)
{
	t_memory	*memory;

	memory = malloc(sizeof(*memory));
	if (!memory)
		return (NULL);
	ft_bzero(memory, sizeof(*memory));
	if (!env[0])
		generate_env(memory);
	else
		create_env(memory, env);
	if (!memory->env)
		return (free_memory(memory), NULL);
	memory->pwd = malloc(PATH_MAX);
	if (!memory->pwd)
		return (free_memory(memory), NULL);
	getcwd(memory->pwd, PATH_MAX);
	memory->oldpwd = ft_strdup(memory->pwd);
	if (!memory->oldpwd)
		return (free_memory(memory), NULL);
	return (memory);
}
