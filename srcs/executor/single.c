/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:41:55 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/10 16:50:50 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	execute_builtin_and_handle_redir(t_command *cmd, t_memory *mem, \
	int saved_fds[2])
{
	bool	is_redir;

	is_redir = false;
	if (cmd->redir_struct)
	{
		is_redir = true;
		handle_redir_in(cmd);
		handle_redir_out(cmd);
	}
	
	execute_builtin(cmd, mem, is_redir, saved_fds);
	if (mem->exit_failed || mem->cd_failed)
	{
		mem->exit_failed = false;
		mem->cd_failed = false;
		mem->exit_status = 1;
	}
	else 
		mem->exit_status = 0;
	if (is_redir)
	{
		dup2(saved_fds[0], STDIN_FILENO);
		dup2(saved_fds[1], STDOUT_FILENO);
	}
	close(saved_fds[0]);
	close(saved_fds[1]);
}

static void	create_process_and_execute(t_command *cmd, t_memory *mem, \
	int *status)
{
	int		pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		if (cmd->redir_struct)
		{
			handle_redir_in(cmd);
			handle_redir_out(cmd);
		}
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			if (contains_slash(cmd->args[0]) || mem->error_code == ERROR_CODE_NO_PATH)
			{
				if (access(cmd->args[0], F_OK) == 0)
				{
					ft_printf("%s: Permission denied\n", STDERR_FILENO, \
						cmd->args[0]);
					exit(PERMISSION_DENIED);
				}
				else
					ft_printf("%s: No such file or directory\n", STDERR_FILENO, \
						cmd->args[0]);
			}
			else
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
			exit(COMMAND_NOT_FOUND);
		}
	}
	if (waitpid(pid, status, 0) == -1)
	{
		perror("waitpid");
		exit(ERROR);
	}
}

void	execute_single_command(t_command *cmd, t_memory *mem, int *status)
{
	int			saved_fds[2];

	if (!cmd->args[0] && !cmd->redir_struct)
		return ;
	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin_and_handle_redir(cmd, mem, saved_fds);
		return ;
	}
	cmd->path = find_path(cmd->args[0], mem);
	create_process_and_execute(cmd, mem, status);
	close(saved_fds[0]);
	close(saved_fds[1]);
	if (WIFEXITED(*status))
		mem->exit_status = WEXITSTATUS(*status);
	else
		mem->exit_status = 1;
}
