/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:41 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:38:07 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], t_memory *mem)
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		cmd->has_redir = true;
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	else
		dup2(fd1[0], STDIN_FILENO);
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		cmd->has_redir = true;
		close(fd2[0]);
		close(fd2[1]);
		handle_redir_out(cmd, mem, cmd->has_child);
	}
	else
		dup2(fd2[1], STDOUT_FILENO);
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}

static void	check_cmd_type_and_run(t_command *cmd, t_memory *mem)
{
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, NULL);
		free_memory(mem);
		close(1);
		close(0);
		exit(0);
	}
	else
		handle_execution(cmd, mem);
}

static int	create_process_and_execute(t_command *cmd, t_memory *mem, \
	int fd1[2], int fd2[2])
{
	int		pid;

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
		handle_fds(cmd, fd1, fd2, mem);
		check_cmd_type_and_run(cmd, mem);
	}
	if (!cmd->has_redir)
	{
		close(fd1[0]);
		close(fd1[1]);
		fd1[0] = fd2[0];
		fd1[1] = fd2[1];
	}
	return (pid);
}

int	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int		fd2[2];

	if (pipe(fd2) == -1)
	{
		perror("pipe");
		end_shell(mem);
	}
	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return (-1);
	cmd->path = find_path(cmd->args[0], mem, cmd);
	return (create_process_and_execute(cmd, mem, fd1, fd2));
}
