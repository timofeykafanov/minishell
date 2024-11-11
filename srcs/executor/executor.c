/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/11 16:53:22 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];
	struct stat	fileStat;
	
	command = memory->commands;
	while (command)
	{
		if (stat(command->args[0], &fileStat) == 0)
		{
			if (S_ISDIR(fileStat.st_mode))
			{
                ft_printf("%s: Is a directory\n", STDERR_FILENO, command->args[0]);
                memory->exit_status = 126;
                return ;
            }
			// else
            // 	perror("stat");
		}
		command = command->next;
	}
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
