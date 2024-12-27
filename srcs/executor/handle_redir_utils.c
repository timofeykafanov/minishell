/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 14:14:14 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/27 21:24:25 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redir_out(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_out == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
			end_shell(mem);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_out);
}

void	redir_append(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
			end_shell(mem);
	}
	if (dup2(fd_out, STDOUT_FILENO == -1))
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_out);
}

void	redir_in(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_in;

	fd_in = open(redir->file_name, O_RDONLY);
	if (fd_in == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
			end_shell(mem);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_in);
}

void	redir_heredoc(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_heredoc;

	fd_heredoc = open(redir->heredoc_file_name, O_RDONLY);
	if (fd_heredoc == -1)
	{
		ft_printf("kinkshell: %s: ", STDERR_FILENO, redir->heredoc_file_name);
		perror("");
		if (has_child)
			end_shell(mem);
	}
	if (dup2(fd_heredoc, STDIN_FILENO) == -1)
	{
		perror("kinkshell: dup2");
		end_shell(mem);
	}
	close(fd_heredoc);
}
