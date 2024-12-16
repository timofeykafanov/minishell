/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:41:55 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/16 14:41:14 by tkafanov         ###   ########.fr       */
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
		handle_redir_in(cmd, mem);
	}
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		is_redir_out = true;
		handle_redir_out(cmd, mem);
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
			handle_redir_in(cmd, mem);
			handle_redir_out(cmd, mem);
		}
		close(saved_fds[0]);
		close(saved_fds[1]);
		if (!cmd->path || !cmd->args[0][0] || (ft_strncmp(cmd->name, "..", 2) == 0 && ft_strlen(cmd->name) == 2))
		{
			close(1);
			close(0);
			if (mem->error_code == ERROR_CODE_NO_PATH || (cmd->name && ft_strchr(cmd->name, '/')))
				ft_printf("%s: No such file or directory\n", STDERR_FILENO, \
						cmd->args[0]);
			else
			{
				if (!cmd->args[0])
				{
					free_memory(mem);
					exit(0);
				}
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
			}
			free_memory(mem);
			exit(COMMAND_NOT_FOUND);
		}
		else if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			close(1);
			close(0);
			if (ft_strlen(cmd->args[0]) == 0)
			{
				free_memory(mem);
				exit(0);
			}
			if (mem->error_code == ERROR_CODE_NO_PATH || ft_strchr(cmd->name, '/'))
			{
				ft_printf("%s: Permission denied\n", STDERR_FILENO, \
					cmd->args[0]);
				free_memory(mem);
				exit(PERMISSION_DENIED);
			}
			else
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, \
					cmd->args[0]);
				free_memory(mem);
				exit(COMMAND_NOT_FOUND);
			}
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
	else
		mem->exit_status = 1;
}
