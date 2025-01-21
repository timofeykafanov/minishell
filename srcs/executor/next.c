/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:41 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/21 14:10:00 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	safe_dup(int fd, int std_fd, t_memory *mem)
{
	if (dup2(fd, std_fd) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
}

static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], t_memory *mem)
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		cmd->has_redir = true;
		close(fd1[0]);
		close(fd1[1]);
		cmd->pipe_fd[0] = fd2[0];
		cmd->pipe_fd[1] = fd2[1];
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	else
		safe_dup(fd1[0], STDIN_FILENO, mem);
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		cmd->has_redir = true;
		close(fd2[0]);
		close(fd2[1]);
		handle_redir_out(cmd, mem, cmd->has_child);
	}
	else
		safe_dup(fd2[1], STDOUT_FILENO, mem);
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}

static void	check_cmd_type_and_run(t_command *cmd, t_memory *mem)
{
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		signal(SIGPIPE, SIG_IGN);
		execute_builtin(cmd, mem, NULL);
		free_memory(mem);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		exit(SUCCESS);
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
		perror("kinkshell: pipe");
		end_shell(mem);
	}
	cmd->path = find_path(cmd->args[0], mem, cmd);
	return (create_process_and_execute(cmd, mem, fd1, fd2));
}
