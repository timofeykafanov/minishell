/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:14 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/04 17:52:53 by sopperma         ###   ########.fr       */
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
	{
		if (dup2(fd1[1], STDOUT_FILENO) == -1)
		{
			perror("kinkshell: dup2");
			end_shell(mem);
		}
	}
	close(fd1[0]);
	close(fd1[1]);
}

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	handle_redirs(cmd, mem, fd1);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		signal(SIGPIPE, SIG_IGN);
		execute_builtin(cmd, mem, NULL);
		close(fd1[0]);
		close(fd1[1]);
		free_memory(mem);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		exit(SUCCESS);
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
