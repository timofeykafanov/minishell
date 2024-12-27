/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:45:07 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 19:31:34 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*check_access(t_memory *memory, char *command, char *path)
{
	char	*res;
	char	*temp;

	if (access(command, F_OK) == 0)
		return (command);
	(void)path;
	res = ft_strjoin(path, "/");
	if (!res)
		end_shell(memory);
	temp = res;
	res = ft_strjoin(temp, command);
	if (!res)
	{
		free(temp);
		end_shell(memory);
	}
	free(temp);
	if (access(res, F_OK) == 0)
		return (res);
	free(res);
	return (NULL);
}

static char	*define_path(t_memory *memory, char *command, char **paths)
{
	int		i;
	char	*res;

	i = 0;
	while (paths[i])
	{
		res = check_access(memory, command, paths[i]);
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
	res = define_path(memory, command, cmd->paths);
	return (res);
}
