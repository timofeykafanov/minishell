/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:59 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/17 18:10:36 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	// dup2(fd1[0], STDIN_FILENO);
	// close(fd1[0]);
	// close(fd1[1]);
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
	{
		if (!cmd->path || !cmd->args[0][0])
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
			ft_printf("%s: Permission denied\n", STDERR_FILENO, \
				cmd->args[0]);
			{
				free_memory(mem);	
				exit(PERMISSION_DENIED);
			}
		}
	}
}

int	execute_last_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	// int	status;

	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return (-1);
	cmd->path = find_path(cmd->args[0], mem, cmd);
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
	// else
	// {
	// 	close(fd1[0]);
	// 	close(fd1[1]);
	// 	// waitpid(pid, &status, 0);
	// 	// mem->exit_status = WEXITSTATUS(status);
	// }
	return (pid);
}
