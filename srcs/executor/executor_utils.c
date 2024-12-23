/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 11:15:57 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:44:32 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	contains_slash(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (command[i] == '/')
			return (true);
		i++;
	}
	return (false);
}

bool	is_builtin(char *command)
{
	if (!ft_strncmp(command, ECHO, 5) || !ft_strncmp(command, CD, 3)
		|| !ft_strncmp(command, PWD, 4) || !ft_strncmp(command, EXPORT, 7)
		|| !ft_strncmp(command, UNSET, 6) || !ft_strncmp(command, ENV, 4)
		|| !ft_strncmp(command, EXIT, 5))
		return (true);
	return (false);
}

void	execute_builtin(t_command *cmd, t_memory *mem, int saved_fds[2])
{
	if (!ft_strncmp(cmd->args[0], ECHO, 5))
		echo(cmd->args);
	else if (!ft_strncmp(cmd->args[0], CD, 4))
		execute_cd(mem, cmd);
	else if (!ft_strncmp(cmd->args[0], PWD, 4))
		execute_pwd(mem);
	else if (!ft_strncmp(cmd->args[0], EXPORT, 7))
		execute_export(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], UNSET, 6))
		unset(mem, cmd->args);
	else if (!ft_strncmp(cmd->args[0], ENV, 4))
		print_env(mem);
	else if (!ft_strncmp(cmd->args[0], EXIT, 5))
		execute_exit(mem, cmd, saved_fds);
}

bool	is_directory(t_memory *memory, t_command *command)
{
	struct stat	file_stat;

	if (command->args[0] && (ft_strncmp(command->args[0], "..", \
		ft_strlen(command->args[0])) == 0
			|| ft_strncmp(command->args[0], ".", \
			ft_strlen(command->args[0])) == 0))
		return (true);
	while (command)
	{
		if (stat(command->args[0], &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
			{
				ft_printf("kinkshell: %s: Is a directory\n", STDERR_FILENO, \
					command->args[0]);
				memory->exit_status = 126;
				return (false);
			}
		}
		command = command->next;
	}
	return (true);
}

void	catch_status(t_memory *memory, int *status)
{
	if (WIFEXITED(*status))
		memory->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		memory->exit_status = WTERMSIG(*status) + 128;
		if (memory->exit_status == 130)
			ft_printf("\n", STDERR_FILENO);
		else if (WCOREDUMP(memory->exit_status))
			ft_printf("Quit (core dumped)\n", STDERR_FILENO);
	}
	else
		memory->exit_status = 1;
}
