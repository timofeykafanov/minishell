/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/08/28 15:39:26 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

void	execute_single_command(t_command *command, t_memory *memory)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	command->path = ft_strjoin("/usr/bin/", command->args[0]);
	if (pid == 0)
	{
		if (execve(command->path, command->args, memory->env) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

void	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	int	status;

	pipe(fd1);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = ft_strjoin("/usr/bin/", cmd->args[0]);
	if (pid == 0)
	{
		dup2(fd1[1], 1);
		close(fd1[0]);
		close(fd1[1]);
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		close(fd1[1]);
		waitpid(pid, &status, 0);
	}
}

void	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2], int fd2[2])
{
	int	pid;
	int	status;

	pipe(fd2);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = ft_strjoin("/usr/bin/", cmd->args[0]);
	if (pid == 0)
	{
		dup2(fd1[0], 0);
		dup2(fd2[1], 1);
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		fd1[0] = fd2[0];
		fd1[1] = fd2[1];
		waitpid(pid, &status, 0);
	}
}

void	execute_last_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = ft_strjoin("/usr/bin/", cmd->args[0]);
	if (pid == 0)
	{
		dup2(fd1[0], 0);
		close(fd1[0]);
		close(fd1[1]);
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		waitpid(pid, &status, 0);
	}

}

void    execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];
	int			fd2[2];

	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory);
	else
	{
		execute_first_command(command, memory, fd1);
		command = command->next;
		while (command->next)
		{
			execute_next_command(command, memory, fd1, fd2);
			command = command->next;
		}
		execute_last_command(command, memory, fd1);
	}
}