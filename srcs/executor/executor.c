/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/03 16:51:52 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_directory(t_memory *memory, t_command *command)
{
	struct stat	fileStat;

	if (command->args[0] && (ft_strncmp(command->args[0], "..", ft_strlen(command->args[0])) == 0
		|| ft_strncmp(command->args[0], ".", ft_strlen(command->args[0])) == 0))
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
	int			*pid;
	int			process_count;
	
	process_count = 1;
	command = memory->commands;
	while (command)
	{
		if (command->next)
			process_count++;
		command = command->next;
	}
	pid = malloc(sizeof(int) * process_count);
	process_count = 0;
	command = memory->commands;
	if (!is_directory(memory, command))
		return ;
	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory);
	else
	{
		pid[process_count++] = execute_first_command(command, memory, fd1);
		// close(fd1[0]);
       	// close(fd1[1]);
		command = command->next;
		while (command->next)
		{
			pid[process_count++] = execute_next_command(command, memory, fd1);
			command = command->next;
			// close(fd1[0]);
       		// close(fd1[1]);
		}
		pid[process_count++] = execute_last_command(command, memory, fd1);
		close(fd1[0]);
        close(fd1[1]);
	}
	for (int i = 0; i < process_count; i++)
	{
		int status;
		if (pid[i] == -1)
			continue ;
		waitpid(pid[i], &status, 0);
		if (i == process_count - 1)
			memory->exit_status = WEXITSTATUS(status);
	}
	free(pid);
}
