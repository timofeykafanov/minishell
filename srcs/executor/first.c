/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:14 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/10 15:44:13 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_command *cmd, t_memory *mem, int fd1[2])
{
	// if (cmd->redir_struct && (cmd->redir_struct->type == T_R_OUT 
	// 	|| cmd->redir_struct->type == T_OUT_APPEND))
	// 	handle_redir_out(cmd);
	// else
	// {
	// 	dup2(fd1[1], STDOUT_FILENO);
	// 	close(fd1[0]);
	// 	close(fd1[1]);
	// }
	if (cmd->redir_struct && has_redir_in(cmd))
		handle_redir_in(cmd);
	else
	{
		// dup2(fd1[0], STDIN_FILENO);
		// close(fd1[0]);
		// close(fd1[1]);
	}
	if (cmd->redir_struct && has_redir_out(cmd))
		handle_redir_out(cmd);
	else
	{
		dup2(fd1[1], STDOUT_FILENO);
		close(fd1[0]);
		close(fd1[1]);
	}
	// close(fd1[0]);
	// close(fd1[1]);
	// if (isatty(STDIN_FILENO)) {
	// 	close(STDIN_FILENO);
	// }
	if (cmd->args[0] && is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, mem, false, NULL);
		exit(0);
	}
	else
	{
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
				ft_printf("%s: command not found\n", \
				STDERR_FILENO, cmd->args[0]);
			exit(COMMAND_NOT_FOUND);
		}
	}
}

int	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (pipe(fd1) == -1)
	{
		perror("pipe");
		exit(1);
	}
	// close(fd1[0]);
    // close(fd1[1]);
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
	return (pid);
}
