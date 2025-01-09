/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:14:14 by tkafanov          #+#    #+#             */
/*   Updated: 2025/01/08 17:33:05 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redir_out(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_out == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
			end_shell(mem);
		else
			return (ERROR);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_out);
	return (SUCCESS);
}

int	redir_append(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
			end_shell(mem);
		else
			return (ERROR);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_out);
	return (SUCCESS);
}

int	redir_in(t_command *cmd, t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_in;

	fd_in = open(redir->file_name, O_RDONLY);
	if (fd_in == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (cmd->pipe_fd[0] != -1 && cmd->pipe_fd[1] != -1)
		{
			close(cmd->pipe_fd[0]);
			close(cmd->pipe_fd[1]);
		}
		if (has_child)
			end_shell(mem);
		else
			return (ERROR);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_in);
	return (SUCCESS);
}

int	redir_heredoc(t_command *cmd, t_redir_out *redir, t_memory *mem, \
	bool has_child)
{
	int	fd_heredoc;

	fd_heredoc = open(redir->heredoc_file_name, O_RDONLY);
	if (fd_heredoc == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->heredoc_file_name);
		perror("");
		if (cmd->pipe_fd[0] != -1 && cmd->pipe_fd[1] != -1)
		{
			close(cmd->pipe_fd[0]);
			close(cmd->pipe_fd[1]);
		}
		if (has_child)
			end_shell(mem);
		else
			return (ERROR);
	}
	if (dup2(fd_heredoc, STDIN_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_heredoc);
	return (SUCCESS);
}
