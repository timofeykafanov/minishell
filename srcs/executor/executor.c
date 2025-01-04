/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/04 17:32:09 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_multiprocess(t_memory *memory, int counter)
{
	int	i;
	int	status;

	i = 0;
	set_signals(WAIT);
	while (i < counter)
	{
		if (memory->pid[i] == -1)
		{
			i++;
			continue ;
		}
		if (waitpid(memory->pid[i], &status, 0) == -1)
		{
			perror("kinkshell: waitpid");
			end_shell(memory);
		}
		i++;
	}
	i--;
	catch_status(memory, &status);
}

static void	count_commands(t_memory *memory)
{
	t_command	*command;
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
}

static void	execute_pipeline(t_memory *memory)
{
	t_command	*cmd;
	int			fd1[2];
	int			process_count;

	cmd = memory->commands;
	memory->is_child = true;
	process_count = 0;
	memory->pid[process_count++] = execute_first_command(cmd, memory, fd1);
	cmd = cmd->next;
	while (cmd->next)
	{
		memory->pid[process_count++] = execute_next_command(cmd, memory, fd1);
		cmd = cmd->next;
	}
	memory->pid[process_count++] = execute_last_command(cmd, memory, fd1);
	close(fd1[0]);
	close(fd1[1]);
	memory->is_child = false;
	handle_multiprocess(memory, process_count);
}

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			status;

	count_commands(memory);
	command = memory->commands;
	if (!is_directory(memory, command))
		return ;
	if (command->next == NULL)
		execute_single_command(command, memory, &status);
	else
		execute_pipeline(memory);
	set_signals(MAIN);
}
