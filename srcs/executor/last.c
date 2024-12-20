/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:59 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 21:38:23 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	else
		dup2(fd1[0], STDIN_FILENO);
	if (cmd->redir_struct && has_redir_out(cmd))
		handle_redir_out(cmd, mem, cmd->has_child);
	close(fd1[0]);
	close(fd1[1]);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, false, NULL);
		free_memory(mem);
		close(1);
		close(0);
		exit(0);
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
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		cmd->has_child = true;
		run_child_process(cmd, mem, fd1);
	}
	return (pid);
}
