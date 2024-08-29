/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/29 16:02:45 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*find_path(char *command, char **paths)
{
	int		i;
	char	*path;

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
