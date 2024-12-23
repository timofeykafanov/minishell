/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:41:55 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/23 15:44:45 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	create_process_and_execute(t_command *cmd, t_memory *mem, \
	int *status, int saved_fds[2])
{
	int	pid;

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
		if (cmd->redir_struct)
			handle_redir(cmd, mem, cmd->has_child);
		close(saved_fds[0]);
		close(saved_fds[1]);
		handle_execution(cmd, mem);
	}
	set_signals(WAIT);
	if (waitpid(pid, status, 0) == -1)
	{
		perror("kinkshell: waitpid");
		end_shell(mem);
	}
	set_signals(MAIN);
}

void	execute_single_command(t_command *cmd, t_memory *mem, int *status)
{
	int	saved_fds[2];

	if (!cmd->args[0] && !cmd->redir_struct)
		return ;
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (saved_fds[0] == -1 || saved_fds[1] == -1)
	{
		perror("kinkshell: dup");
		exit(EXIT_FAILURE);
	}
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin_and_handle_redir(cmd, mem, saved_fds);
		close(saved_fds[0]);
		close(saved_fds[1]);
		return ;
	}
	cmd->path = find_path(cmd->name, mem, cmd);
	close(saved_fds[0]);
	close(saved_fds[1]);
	create_process_and_execute(cmd, mem, status, saved_fds);
	catch_status(mem, status);
}
