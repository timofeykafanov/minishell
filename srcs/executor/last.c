/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:59 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/02 17:35:09 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	dup2(fd1[0], STDIN_FILENO);
	close(fd1[0]);
	close(fd1[1]);
	if (cmd->redir_struct && has_redir_in(cmd))
		handle_redir_in(cmd);
	else
		dup2(fd1[0], STDIN_FILENO);
	close(fd1[0]);
	close(fd1[1]);
	if (cmd->redir_struct && has_redir_out(cmd))
		handle_redir_out(cmd);

	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, NULL);
		exit(0);
	}
	else
	{
		if (execve(cmd->path, cmd->args, mem->env) == -1)
		{
			if (contains_slash(cmd->args[0]))
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
}

void	execute_last_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	int	status;

	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return ;
	cmd->path = find_path(cmd->args[0], mem);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		run_child_process(cmd, mem, fd1);
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		waitpid(pid, &status, 0);
		mem->exit_status = WEXITSTATUS(status);
	}
}
