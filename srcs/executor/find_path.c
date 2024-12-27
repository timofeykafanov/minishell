/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 14:59:31 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*check_access(char *command, char **paths, int i)
{
	char	*path;
	char	*temp;

	if (access(command, F_OK) == 0)
		return (command);
	path = ft_strjoin(paths[i], "/");
	if (!path)
		end_shell(NULL);
	temp = path;
	path = ft_strjoin(temp, command);
	if (!path)
	{
		free(temp);
		end_shell(NULL);
	}
	free(temp);
	if (access(path, F_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

static char	*define_path(char *command, char **paths)
{
	int		i;
	char	*res;

	i = 0;
	while (paths[i])
	{
		res = check_access(command, paths, i);
		if (res)
			return (res);
		i++;
	}
	return (NULL);
}

static char	*handle_cmd_with_slash(t_memory *memory, char *command)
{
	char	*res;

	if (access(command, F_OK) == 0)
	{
		res = ft_strdup(command);
		if (!res)
			end_shell(memory);
		return (res);
	}
	return (NULL);
}

char	*find_path(char *command, t_memory *memory, t_command *cmd)
{
	char	*res;

	if (!command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (handle_cmd_with_slash(memory, command));
	cmd->env_path = find_env_value(memory, "PATH");
	if (ft_strlen(cmd->env_path) == 0)
		set_error_code(PATH, ERROR_CODE_NO_PATH, memory);
	if (access(command, F_OK) == 0)
	{
		res = ft_strjoin("./", command);
		if (!res)
			end_shell(memory);
		return (res);
	}
	if (!cmd->env_path)
		return (NULL);
	cmd->paths = ft_split(cmd->env_path, ':');
	if (!cmd->paths)
		end_shell(memory);
	res = define_path(command, cmd->paths);
	return (res);
}
