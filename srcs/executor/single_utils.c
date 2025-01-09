/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 15:40:45 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/08 17:33:15 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_redirs(t_memory *mem, t_command *cmd)
{
	if (cmd->redir_struct && (has_redir_in(cmd) || has_redir_out(cmd)))
	{
		if (handle_redir(cmd, mem, cmd->has_child) == ERROR)
			return (ERROR);
		if (has_redir_in(cmd))
			cmd->is_redir_in = true;
		if (has_redir_out(cmd))
			cmd->is_redir_out = true;
	}
	return (SUCCESS);
}

static void	handle_redir_back(t_memory *mem, t_command *cmd, int saved_fds[2])
{
	if (cmd->is_redir_in)
	{
		cmd->is_redir_in = false;
		if (dup2(saved_fds[0], STDIN_FILENO) == -1)
		{
			perror("kinkshell: dup2 in");
			free_memory(mem);
			exit(ERROR);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
	if (cmd->is_redir_out)
	{
		cmd->is_redir_out = false;
		if (dup2(saved_fds[1], STDOUT_FILENO) == -1)
		{
			perror("kinkshell: dup2 out");
			free_memory(mem);
			exit(ERROR);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
	}
}

void	execute_builtin_and_handle_redir(t_command *cmd, t_memory *mem, \
	int saved_fds[2])
{
	if (handle_redirs(mem, cmd) == ERROR)
	{
		mem->exit_status = 1;
		handle_redir_back(mem, cmd, saved_fds);
		return ;
	}
	execute_builtin(cmd, mem, saved_fds);
	if (mem->cd_failed)
	{
		mem->cd_failed = false;
		mem->exit_status = 1;
	}
	else if (mem->exit_failed)
	{
		mem->exit_failed = false;
		if (mem->exit_status == 0)
			mem->exit_status = 1;
	}
	else
		mem->exit_status = 0;
	handle_redir_back(mem, cmd, saved_fds);
	close(saved_fds[0]);
	close(saved_fds[1]);
}
