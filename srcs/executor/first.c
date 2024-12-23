/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:14 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:33:11 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_redirs(t_command *cmd, t_memory *mem, int fd1[2])
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_out(cmd, mem, cmd->has_child);
	}
	else
		dup2(fd1[1], STDOUT_FILENO);
	close(fd1[0]);
	close(fd1[1]);
}

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	handle_redirs(cmd, mem, fd1);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, NULL);
		close(fd1[0]);
		close(fd1[1]);
		free_memory(mem);
		close(1);
		close(0);
		exit(0);
	}
	else
		handle_execution(cmd, mem);
}

int	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (pipe(fd1) == -1)
	{
		perror("kinkshell: pipe");
		end_shell(mem);
	}
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
