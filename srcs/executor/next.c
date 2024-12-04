/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:41 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/04 16:32:05 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], bool *is_redir)
// {
// 	dup2(fd1[0], STDIN_FILENO);
// 	close(fd1[0]);
// 	close(fd1[1]);
// 	if (cmd->redir_struct && (cmd->redir_struct->type == T_R_OUT 
// 		|| cmd->redir_struct->type == T_OUT_APPEND))
// 	{
// 		*is_redir = true;
// 		handle_redir_out(cmd);
// 	}
// 	else
// 	{
// 		dup2(fd2[1], STDOUT_FILENO);
// 		close(fd2[0]);
// 		close(fd2[1]);
// 	}
// }
static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], bool *is_redir)
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		*is_redir = true;
		handle_redir_in(cmd);
	}
	else
		dup2(fd1[0], STDIN_FILENO);
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		*is_redir = true;
		handle_redir_out(cmd);
	}
	else
		dup2(fd2[1], STDOUT_FILENO);
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}

static void	check_cmd_type_and_run(t_command *cmd, t_memory *mem)
{
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

static int	create_process_and_execute(t_command *cmd, t_memory *mem, \
	int fd1[2], int fd2[2])
{
	int		pid;
	bool	is_redir;

	is_redir = false;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid == 0)
	{
		handle_fds(cmd, fd1, fd2, &is_redir);
		check_cmd_type_and_run(cmd, mem);
	}
	else
	{
		if (!is_redir)
		{
			close(fd1[0]);
			close(fd1[1]);
			fd1[0] = fd2[0];
			fd1[1] = fd2[1];
		}
	}
	return (pid);
}

int	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int		fd2[2];

	if (pipe(fd2) == -1)
	{
		perror("pipe");
		exit(1);
	}
	// close(fd1[0]);
    // close(fd1[1]);
	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return (-1);
	cmd->path = find_path(cmd->args[0], mem);
	return (create_process_and_execute(cmd, mem, fd1, fd2));
}
