/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:14 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/13 18:01:48 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_in(cmd, mem);
	}

	if (cmd->redir_struct && has_redir_out(cmd))
	{
		close(fd1[0]);
		close(fd1[1]);	
		handle_redir_out(cmd, mem);
	}
	else
		dup2(fd1[1], STDOUT_FILENO);
	close(fd1[0]);
	close(fd1[1]);	

	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, false, NULL);
		close(fd1[0]);
		close(fd1[1]);
		free_memory(mem);
		close(1);
		close(0);
		exit(0);
	}
	else
	{
		if (!cmd->path || !cmd->args[0][0])
		{
			close(1);
			close(0);
			if (mem->error_code == ERROR_CODE_NO_PATH || (cmd->name && ft_strchr(cmd->name, '/')))
				ft_printf("%s: No such file or directory\n", STDERR_FILENO, \
						cmd->args[0]);
			else
			{
				if (!cmd->args[0])
				{
					free_memory(mem);
					exit(0);
				}
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
			}
			free_memory(mem);
			exit(COMMAND_NOT_FOUND);
		}
		else if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			close(1);
			close(0);
			if (ft_strlen(cmd->args[0]) == 0)
			{
				free_memory(mem);
				exit(0);
			}
			ft_printf("%s: Permission denied\n", STDERR_FILENO, \
				cmd->args[0]);
			{
				free_memory(mem);	
				exit(PERMISSION_DENIED);
			}
		}
	}
}

int	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (pipe(fd1) == -1)
	{
		perror("pipe");
		exit(1);
	}
	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return (-1);
	cmd->path = find_path(cmd->args[0], mem, cmd);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		run_child_process(cmd, mem, fd1);
	return (pid);
}
