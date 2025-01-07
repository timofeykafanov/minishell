/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:59 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/07 19:27:30 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_redirs(t_command *cmd, t_memory *mem, int fd1[2])
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		// close(fd1[0]);
		// close(fd1[1]);
		cmd->pipe_fd[0] = fd1[0];
		cmd->pipe_fd[1] = fd1[1];
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	else
	{
		if (dup2(fd1[0], STDIN_FILENO) == -1)
		{
			perror("kinkshell: dup2");
			end_shell(mem);
		}
	}
	if (cmd->redir_struct && has_redir_out(cmd))
		handle_redir_out(cmd, mem, cmd->has_child);
	close(fd1[0]);
	close(fd1[1]);
}

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	handle_redirs(cmd, mem, fd1);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, NULL);
		free_memory(mem);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		exit(SUCCESS);
	}
	else
		handle_execution(cmd, mem);
}

int	execute_last_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return (-1);
	cmd->path = find_path(cmd->args[0], mem, cmd);
	set_signals(CHILD);
	pid = fork();
	if (pid == -1)
	{
		perror("kinkshell: fork");
		end_shell(mem);
	}
	if (pid == 0)
	{
		cmd->has_child = true;
		run_child_process(cmd, mem, fd1);
	}
	return (pid);
}
