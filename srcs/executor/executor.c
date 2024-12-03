/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/03 19:07:10 by tkafanov         ###   ########.fr       */
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
	int 		status;
	int			counter;
	
	counter = 1;
	command = memory->commands;
	while (command)
	{
		if (command->next)
			counter++;
		command = command->next;
	}
	pid = malloc(sizeof(int) * counter);
	process_count = 0;
	command = memory->commands;
	if (!is_directory(memory, command))
		return ;
	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory, &status);
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
	int i;
	i = process_count - 1;
	if (counter > 1)
	{
		while (i >= 0)
		{
			if (pid[i] == -1)
				continue ;
			// printf("pid: %d\n", pid[i]);
			// if (i > 0)
			waitpid(pid[i], &status, WNOHANG);
				// usleep(1000);
			// else
			i--;
			// else
			// 	waitpid(pid[i], &status, 0);
			// if (i == process_count)
			// 	memory->exit_status = WEXITSTATUS(status);
		}
		memory->exit_status = WEXITSTATUS(status);
	}
	free(pid);
}
