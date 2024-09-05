/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:04:36 by tkafanov          #+#    #+#             */
/*   Updated: 2024/09/05 11:31:23 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

void handle_redir(t_command *cmd)
{
	int			fd_in;
	int			fd_out;
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
        {
			fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
            if (fd_out == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if (redir->type == T_OUT_APPEND)
        {
            fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd_out == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if (redir->type == T_R_IN)
        {
            fd_in = open(redir->file_name, O_RDONLY);
            if (fd_in == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
		redir = redir->next;
	}
}

void	handle_fake_redir(t_command *cmd)
{
	int			fd_in;
	int			fd_out;
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
        {
			fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
            if (fd_out == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            close(fd_out);
        }
        if (redir->type == T_OUT_APPEND)
        {
            fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd_out == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            close(fd_out);
        }
        if (redir->type == T_R_IN)
        {
            fd_in = open(redir->file_name, O_RDONLY);
            if (fd_in == -1)
            {
                ft_printf("minishell: %s: ", STDERR_FILENO, redir->file_name);
				perror("");
                exit(1);
            }
            close(fd_in);
        }
		redir = redir->next;
	}
}

void	execute_single_command(t_command *cmd, t_memory *mem)
{
	int		pid;
	int		status;
	bool	is_redir = false;
	int		saved_fds[2];

	saved_fds[0] = dup(STDIN_FILENO);
	saved_fds[1] = dup(STDOUT_FILENO);
	// if (is_cd_or_exit(cmd->args[0]))
	// {
	// 	if (cmd->redir_struct)
	// 		handle_fake_redir(cmd);
	// 	execute_cd_or_exit(cmd, mem);
	// 	mem->exit_status = 0;
	// 	return ;
	// }
	if (is_builtin(cmd->args[0]))
	{
		if (cmd->redir_struct)
		{
			is_redir = true;
			handle_redir(cmd);
		}
		execute_builtin(cmd, mem, is_redir, saved_fds);
		mem->exit_status = 0;
		if (is_redir)
		{
			dup2(saved_fds[0], STDIN_FILENO);
			dup2(saved_fds[1], STDOUT_FILENO);
			close(saved_fds[0]);
			close(saved_fds[1]);
		}
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem);
	if (pid == 0)
	{
		if (cmd->redir_struct)
			handle_redir(cmd);
		// if (is_builtin(cmd->args[0]))
		// {
		// 	execute_builtin(cmd, mem);
		// 	exit(0);
		// }
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(COMMAND_NOT_FOUND);
			}
		}
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
            perror("waitpid");
            exit(ERROR);
        }
		if (WIFEXITED(status))
			mem->exit_status = WEXITSTATUS(status);
		else
			mem->exit_status = 1;
	}
}

void	execute_first_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;

	if (pipe(fd1) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem);
	if (pid == 0)
	{
		if (cmd->redir_struct)
			handle_redir(cmd);
		else
		{
			dup2(fd1[1], STDOUT_FILENO);
			close(fd1[0]);
			close(fd1[1]);
		}
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem, false, NULL);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(COMMAND_NOT_FOUND);
			}
		}
	}
}
// TODO: red does not work with middle commands
void	execute_next_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int		pid;
	int		fd2[2];
	bool	is_redir = false;

	if (pipe(fd2) == -1)
	{
		perror("pipe");
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem);
	if (pid == 0)
	{
		dup2(fd1[0], STDIN_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		if (cmd->redir_struct)
		{
			is_redir = true;
			handle_redir(cmd);
		}
		else
		{
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[0]);
			close(fd2[1]);
		}
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem, false, NULL);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(COMMAND_NOT_FOUND);
			}
		}
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

void	execute_last_command(t_command *cmd, t_memory *mem, int fd1[2])
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	cmd->path = find_path(cmd->args[0], mem);
	if (pid == 0)
	{
		dup2(fd1[0], STDIN_FILENO);
		close(fd1[0]);
		close(fd1[1]);
		if (cmd->redir_struct)
			handle_redir(cmd);
		if (is_builtin(cmd->args[0]))
		{
			execute_builtin(cmd, mem, false, NULL);
			exit(0);
		}
		else
		{
			if (execve(cmd->path, cmd->args, mem->env) == -1)
			{
				ft_printf("%s: command not found\n", STDERR_FILENO, cmd->args[0]);
				exit(COMMAND_NOT_FOUND);
			}
		}
	}
	else
	{
		close(fd1[0]);
		close(fd1[1]);
		waitpid(pid, &status, 0);
		mem->exit_status = WEXITSTATUS(status);
	}
}

void	execute_commands(t_memory *memory)
{
	t_command	*command;
	int			fd1[2];

	command = memory->commands;
	if (command->next == NULL)
		execute_single_command(command, memory);
	else
	{
		execute_first_command(command, memory, fd1);
		command = command->next;
		while (command->next)
		{
			execute_next_command(command, memory, fd1);
			command = command->next;
		}
		execute_last_command(command, memory, fd1);
	}
}
