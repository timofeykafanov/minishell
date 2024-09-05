/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/05 17:28:35 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

char	*find_path(char *command, t_memory *memory)
{
	int		i;
	char	**paths;
	char	*path;
	char	*env_path;
	char	*temp;

	env_path = get_env_var(memory, "PATH=");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if (access(command, F_OK) == 0)
		{
			free_paths(paths);	
			return (command);
		}
		path = ft_strjoin(paths[i], "/");
		temp = path;
		path = ft_strjoin(temp, command);
		free(temp);
		if (access(path, F_OK) == 0)
		{
			free_paths(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_paths(paths);
	return (NULL);
}
