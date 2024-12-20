/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:41:55 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/20 21:33:25 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execute_builtin_and_handle_redir(t_command *cmd, t_memory *mem, \
	int saved_fds[2])
{
	bool	is_redir_out;
	bool	is_redir_in;

	is_redir_in = false;
	is_redir_out = false;
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		is_redir_in = true;
		handle_redir_in(cmd, mem, cmd->has_child);
	}
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		is_redir_out = true;
		handle_redir_out(cmd, mem, cmd->has_child);
	}
	
	execute_builtin(cmd, mem, is_redir_out, is_redir_in, saved_fds);
	if (mem->exit_failed || mem->cd_failed)
	{
		mem->exit_failed = false;
		mem->cd_failed = false;
		mem->exit_status = 1;
	}
	else 
		mem->exit_status = 0;
	if (is_redir_in)
	{
		is_redir_in = false;
		dup2(saved_fds[0], STDIN_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (is_redir_out)
	{
		is_redir_out = false;
		dup2(saved_fds[1], STDOUT_FILENO);
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	close(saved_fds[0]);
	close(saved_fds[1]);
}

static void	create_process_and_execute(t_command *cmd, t_memory *mem, \
	int *status, int saved_fds[2])
{
	int	pid;

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
		if (cmd->redir_struct)
		{
			handle_redir_in(cmd, mem, cmd->has_child);
			handle_redir_out(cmd, mem, cmd->has_child);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
		handle_execution(cmd, mem);
	}
	set_signals(WAIT);
	if (waitpid(pid, status, 0) == -1)
	{
		perror("waitpid");
		free_memory(mem);
		exit(ERROR);
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
		perror("dup");
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
	if (WIFEXITED(*status))
		mem->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		mem->exit_status = WTERMSIG(*status) + 128;
		if (mem->exit_status == 130)
			ft_printf("\n", STDERR_FILENO);
		else if (WCOREDUMP(mem->exit_status))
			ft_printf("Quit (core dumped)\n", STDERR_FILENO);
	}
	else
		mem->exit_status = 1;
}
