/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 23:27:04 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_directory(t_memory *memory, t_command *command)
{
	struct stat	file_stat;

	if (command->args[0] && (ft_strncmp(command->args[0], "..", ft_strlen(command->args[0])) == 0
		|| ft_strncmp(command->args[0], ".", ft_strlen(command->args[0])) == 0))
		return (true);
	while (command)
	{
		if (stat(command->args[0], &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
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

static void	handle_multiprocess(t_memory *memory, int counter, int *status)
{
	int	i;

	i = 0;
	set_signals(WAIT);
	while (i < counter)
	{
		if (memory->pid[i] == -1)
			continue ;
		waitpid(memory->pid[i], status, 0);
		i++;
	}
	i--;
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
		memory->is_child = true;
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
		memory->is_child = false;
	}
	if (counter > 1)
		handle_multiprocess(memory, process_count, &status);
	set_signals(MAIN);
}


// static void	execute_miltiple_commands(t_memory *memory, t_command *cmd,
// 	int *fd1, int *counter)
// {
// 	memory->is_child = true;
// 	memory->pid[*counter++] = execute_first_command(cmd, memory, fd1);
// 	cmd = cmd->next;
// 	while (cmd->next)
// 	{
// 		memory->pid[*counter++] = execute_next_command(cmd, memory, fd1);
// 		cmd = cmd->next;
// 	}
// 	memory->pid[*counter++] = execute_last_command(cmd, memory, fd1);
// 	close(fd1[0]);
// 	close(fd1[1]);
// 	memory->is_child = false;
// }

// static void	handle_multiprocess(t_memory *memory, int counter, int *status)
// {
// 	int	i;

// 	i = 0;
// 	set_signals(WAIT);
// 	while (i < counter)
// 	{
// 		if (memory->pid[i] == -1)
// 			continue ;
// 		waitpid(memory->pid[i], status, 0);
// 		i++;
// 	}
// 	i--;
// 	if (WIFEXITED(*status))
// 		memory->exit_status = WEXITSTATUS(*status);
// 	else if (WIFSIGNALED(*status))
// 	{
// 		memory->exit_status = WTERMSIG(*status) + 128;
// 		if (memory->exit_status == 130)
// 			ft_printf("\n", STDERR_FILENO);
// 		else if (WCOREDUMP(memory->exit_status))
// 			ft_printf("Quit (core dumped)\n", STDERR_FILENO);
// 	}
// 	else
// 		memory->exit_status = 1;
// }

// void	execute_commands(t_memory *memory)
// {
// 	t_command	*command;
// 	int			fd1[2];
// 	int			counter;
// 	int			i;
// 	int			status;

// 	i = 1;
// 	command = memory->commands;
// 	while (command)
// 	{
// 		if (command->next)
// 			i++;
// 		command = command->next;
// 	}
// 	memory->pid = malloc(sizeof(pid_t) * i);
// 	if (!memory->pid)
// 	{
// 		free_memory(memory);	
// 		exit(1);
// 	}
// 	counter = 0;
// 	command = memory->commands;
// 	if (!is_directory(memory, command))
// 		return ;
// 	command = memory->commands;
// 	if (command->next == NULL)
// 		execute_single_command(command, memory, &status);
// 	else
// 		execute_miltiple_commands(memory, command, fd1, &counter);
// 	if (i > 1)
// 		handle_multiprocess(memory, counter, &status);
// 	set_signals(MAIN);
// }
