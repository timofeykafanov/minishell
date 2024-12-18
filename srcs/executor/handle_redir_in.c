/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkafanov <tkafanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:55:23 by tkafanov          #+#    #+#             */
/*   Updated: 2024/12/17 18:12:06 by tkafanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdbool.h>

static void	redir_in(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_in;

	fd_in = open(redir->file_name, O_RDONLY);
	if (fd_in == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->file_name);
		perror("");
		if (has_child)
		{
			free_memory(mem);
			exit(1);
		}
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

static void redir_heredoc(t_redir_out *redir, t_memory *mem, bool has_child)
{
	int	fd_heredoc;
	
	fd_heredoc = open(redir->heredoc_file_name, O_RDONLY);
	if (fd_heredoc == -1)
	{
		ft_printf("%s: ", STDERR_FILENO, redir->heredoc_file_name);
		perror("");
		if (has_child)
		{
			free_memory(mem);
			exit(1);
		}
	}
	dup2(fd_heredoc, STDIN_FILENO);
	close(fd_heredoc);
}

void	handle_redir_in(t_command *cmd, t_memory *mem, bool has_child)
{
	t_redir_out	*redir;

	redir = cmd->redir_struct;
	while (redir)
	{
		if (redir->type == T_R_IN)
			redir_in(redir, mem, has_child);
		if (redir->type == T_HEREDOC)
			redir_heredoc(redir, mem, has_child);
		redir = redir->next;
	}
}