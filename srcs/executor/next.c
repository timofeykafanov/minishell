/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:41 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/12 09:36:21 by tkafanov         ###   ########.fr       */
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
static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], bool *is_redir, t_memory *mem)
{
	if (cmd->redir_struct && has_redir_in(cmd))
	{
		*is_redir = true;
		close(fd1[0]);
		close(fd1[1]);
		handle_redir_in(cmd, mem);
	}
	else
		dup2(fd1[0], STDIN_FILENO);
	if (cmd->redir_struct && has_redir_out(cmd))
	{
		*is_redir = true;
		close(fd2[0]);
		close(fd2[1]);
		handle_redir_out(cmd, mem);
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
		if (!cmd->path || !cmd->args[0][0])
		{
			if (mem->error_code == ERROR_CODE_NO_PATH || ft_strchr(cmd->name, '/'))
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
		handle_fds(cmd, fd1, fd2, &is_redir, mem);
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
