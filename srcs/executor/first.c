/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:14 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/06 11:11:05 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	if (cmd->redir_struct)
		handle_redir(cmd);
	else
	{
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[0]);
		close(fd1[1]);
	}
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, NULL);
		exit(0);
	}
	else
	{
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			ft_printf("%s: command not found\n", \
				STDERR_FILENO, cmd->args[0]);
			exit(COMMAND_NOT_FOUND);
		}
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
	cmd->path = find_path(cmd->args[0], mem);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		run_child_process(cmd, mem, fd1);
}
