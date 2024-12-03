/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/03 18:19:13 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static char	*check_access(char *command, char **paths, int i)
{
	char	*path;
	char	*temp;

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
	return (NULL);
}

// static char	*check_in_current_dir(char *command)
// {
// 	if (access(command, F_OK) == 0)
// 		return (ft_strdup(command));
// 	return (NULL);
// }

char	*find_path(char *command, t_memory *memory)
{
	int		i;
	char	*env_path;
	char	**paths;
	char	*res;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
	{
		if (access(command, F_OK) == 0)
			return (ft_strdup(command));
		return (NULL);
	}
	if (access(command, F_OK) == 0)
		return (ft_strdup(command));
	env_path = get_env_var(memory, "PATH=");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		res = check_access(command, paths, i);
		if (res)
			return (ft_strdup(res));
		i++;
	}
	free_paths(paths);
	return (NULL);
}
