/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/02 15:49:23 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_single_command(t_command *cmd, t_memory *mem)
{
	int	pid;
	int	status;

	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem->path);
	if (pid == 0)
	{
		if (is_builtin(cmd->args[0]))
			execute_builtin(cmd, mem);
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(1);
			}
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		mem->exit_status = WEXITSTATUS(status);	
	}
}

void	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (pipe(fd1) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem->path);
	if (pid == 0)
	{
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(1);
			}
		}
	}
}

void	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	int	fd2[2];

	if (pipe(fd2) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem->path);
	if (pid == 0)
	{
		dup2(fd1[0], STDIN_FILENO);
		dup2(fd2[1], STDOUT_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(1);
			}
		}
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		fd1[0] = fd2[0];
		fd1[1] = fd2[1];
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
	cmd->path = find_path(cmd->args[0], mem->path);
	if (pid == 0)
	{
		dup2(fd1[0], STDIN_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(1);
			}
		}
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		waitpid(pid, &status, 0);
		mem->exit_status = WEXITSTATUS(status);
	}
}

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];

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
