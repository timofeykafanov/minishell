/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sopperma <sopperma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:42:41 by tkafanov          #+#    #+#             */
/*   Updated: 2024/11/26 18:47:06 by sopperma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_fds(t_command *cmd, int fd1[2], int fd2[2], bool *is_redir)
{
	dup2(fd1[0], STDIN_FILENO);
	close(fd1[0]);
	close(fd1[1]);
	if (cmd->redir_struct)
	{
		*is_redir = true;
		handle_redir(cmd);
	}
	else
	{
		dup2(fd2[1], STDOUT_FILENO);
		close(fd2[0]);
		close(fd2[1]);
	}
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
				ft_printf("%s: No such file or directory\n", STDERR_FILENO, \
					cmd->args[0]);
			else
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
			exit(COMMAND_NOT_FOUND);
		}
	}
}

static void	create_process_and_execute(t_command *cmd, t_memory *mem, \
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
}

// TODO: red does not work with middle commands
void	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int		fd2[2];

	if (pipe(fd2) == -1)
	{
		perror("pipe");
		exit(1);
	}
	if (cmd->args[0] && ft_strlen(cmd->args[0]) == 0)
		return ;
	cmd->path = find_path(cmd->args[0], mem);
	create_process_and_execute(cmd, mem, fd1, fd2);
}
