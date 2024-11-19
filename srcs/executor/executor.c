/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/19 15:56:33 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_directory(t_memory *memory, t_command *command)
{
	struct stat	fileStat;

	if (command->args[0] && ft_strncmp(command->args[0], "..", ft_strlen(command->args[0])) == 0)
		return (true);
	while (command)
	{
		if (stat(command->args[0], &fileStat) == 0)
		{
			if (S_ISDIR(fileStat.st_mode))
			{
				ft_printf("%s: Is a directory\n", STDERR_FILENO, command->args[0]);
				memory->exit_status = 126;
				return (false);
			}
			// else
			// 	perror("stat");
		}
		command = command->next;
	}
	return (true);
}

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];
	
	command = memory->commands;
	if (!is_directory(memory, command))
		return ;
	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory);
	else
	{
		execute_first_command(command, memory, fd1);
		command = command->next;
		while (command->next)
		{
			execute_next_command(command, memory, fd1);
			command = command->next;
		}
		execute_last_command(command, memory, fd1);
	}
}
