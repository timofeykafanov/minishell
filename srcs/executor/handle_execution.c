/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 21:25:20 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:22:46 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_error_before_execve(t_command *cmd, t_memory *mem)
{
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	if (mem->error_code == ERROR_CODE_NO_PATH
		|| (cmd->name && ft_strchr(cmd->name, '/')))
		ft_printf("kinkshell: %s: No such file or directory\n", STDERR_FILENO, \
				cmd->args[0]);
	else
	{
		if (!cmd->args[0])
		{
			free_memory(mem);
			exit(SUCCESS);
		}
		ft_printf("kinkshell: %s: command not found\n", \
			STDERR_FILENO, cmd->args[0]);
	}
	free_memory(mem);
	exit(COMMAND_NOT_FOUND);
}

static void	handle_error_after_execve(t_command *cmd, t_memory *mem)
{
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	if (ft_strlen(cmd->args[0]) == 0)
	{
		free_memory(mem);
		exit(SUCCESS);
	}
	if (mem->error_code == ERROR_CODE_NO_PATH || ft_strchr(cmd->name, '/'))
	{
		ft_printf("kinkshell: %s: Permission denied\n", STDERR_FILENO, \
			cmd->args[0]);
		free_memory(mem);
		exit(PERMISSION_DENIED);
	}
	else
	{
		ft_printf("kinkshell: %s: command not found\n", STDERR_FILENO, \
			cmd->args[0]);
		free_memory(mem);
		exit(COMMAND_NOT_FOUND);
	}
}

void	handle_execution(t_command *cmd, t_memory *mem)
{
	if (!cmd->path || !cmd->args[0][0]
		|| (ft_strncmp(cmd->name, "..", 2) == 0 && ft_strlen(cmd->name) == 2))
		handle_error_before_execve(cmd, mem);
	else if (execve(cmd->path, cmd->args, mem->env) == -1)
		handle_error_after_execve(cmd, mem);
}
