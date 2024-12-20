/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 13:05:08 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <unistd.h>

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
		}
		command = command->next;
	}
	return (true);
}

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];
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
	memory->pid = malloc(sizeof(pid_t) * counter);
	process_count = 0;
	command = memory->commands;
	if (!is_directory(memory, command))
		return ;
	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory, &status);
	else
	{
		memory->pid[process_count++] = execute_first_command(command, memory, fd1);
		command = command->next;
		while (command->next)
		{
			memory->pid[process_count++] = execute_next_command(command, memory, fd1);
			command = command->next;
		}
		memory->pid[process_count++] = execute_last_command(command, memory, fd1);
		close(fd1[0]);
		close(fd1[1]);
	}
	int i;
	i = 0;
	if (counter > 1)
	{
		set_signals(WAIT);
		while (i < process_count)
		{
			if (memory->pid[i] == -1)
				continue ;
			waitpid(memory->pid[i], &status, 0);
			i++;
		}
		i--;
		if (WIFEXITED(status))
			memory->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			memory->exit_status = WTERMSIG(status) + 128;
			if (memory->exit_status == 130)
				ft_printf("\n", STDERR_FILENO);
			else if (WCOREDUMP(memory->exit_status))
				ft_printf("Quit (core dumped)\n", STDERR_FILENO);
		}
		else
			memory->exit_status = 1;
		set_signals(MAIN);
	}
}
