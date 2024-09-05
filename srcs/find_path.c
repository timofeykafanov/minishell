/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/05 13:52:20 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *command, t_memory *memory)
{
	int		i;
	char	**paths;
	char	*path;
	char	*env_path;

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
			return (command);
		path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path, command);
		if (access(path, F_OK) == 0)
			return (path);
		i++;
	}
	return (NULL);
}
