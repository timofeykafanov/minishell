/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:59 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/11 20:23:11 by tkafanov         ###   ########.fr       */
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
		handle_redir_in(cmd);
	}
	else
		dup2(fd1[0], STDIN_FILENO);
	if (cmd->redir_struct && has_redir_out(cmd))
		handle_redir_out(cmd);
	close(fd1[0]);
	close(fd1[1]);

	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, NULL);
		exit(0);
	}
	else
	{
		if (!cmd->path)
		{
			if (mem->error_code == ERROR_CODE_NO_PATH)
				ft_printf("%s: No such file or directory\n", STDERR_FILENO, \
						cmd->args[0]);
			else
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
			free_memory(mem);
			exit(COMMAND_NOT_FOUND);
		}
		else if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
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
	cmd->path = find_path(cmd->args[0], mem);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
		run_child_process(cmd, mem, fd1);
	// else
	// {
	// 	close(fd1[0]);
	// 	close(fd1[1]);
	// 	// waitpid(pid, &status, 0);
	// 	// mem->exit_status = WEXITSTATUS(status);
	// }
	return (pid);
}
