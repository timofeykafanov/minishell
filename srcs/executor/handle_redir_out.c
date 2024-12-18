/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 10:26:44 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/17 18:07:34 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	redir_out(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_out == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
		{
			free_memory(mem);
			exit(1);
		}
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

static void	redir_append(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_out;

	fd_out = open(redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
		{
			free_memory(mem);
			exit(1);
		}
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

void	handle_redir_out(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_OUT)
			redir_out(redir, mem, has_child);
		if (redir->type == T_OUT_APPEND)
			redir_append(redir, mem, has_child);
		redir = redir->next;
	}
}
